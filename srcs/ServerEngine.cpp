/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerEngine.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 18:11:38 by mababou           #+#    #+#             */
/*   Updated: 2022/09/26 18:42:07 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ServerEngine.hpp"

void ServerEngine::_init_dictionary()
{
	err_dictionary.insert(std::make_pair(200, "OK"));
	err_dictionary.insert(std::make_pair(201, "Created"));
	err_dictionary.insert(std::make_pair(301, "Moved Permanently"));
	err_dictionary.insert(std::make_pair(302, "Found"));
	err_dictionary.insert(std::make_pair(307, "Temporary Redirect"));
	err_dictionary.insert(std::make_pair(308, "Permanent Redirect"));
	err_dictionary.insert(std::make_pair(400, "Bad Request"));
	err_dictionary.insert(std::make_pair(403, "Forbidden"));
	err_dictionary.insert(std::make_pair(404, "Resource Not Found"));
	err_dictionary.insert(std::make_pair(405, "Method Not Allowed"));
	err_dictionary.insert(std::make_pair(500, "Internal Server Error"));
}

static int fd_set_blocking(int fd, int blocking)
{
	/* Save the current flags */
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
		return 0;

	if (blocking)
		flags &= ~O_NONBLOCK;
	else
		flags |= O_NONBLOCK;
	return fcntl(fd, F_SETFL, flags) != -1;
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ServerEngine::ServerEngine(Server &server) : _server(server), _virtual_server(0), _req(NULL), _resp(NULL)
{
	// build error dictionary
	_init_dictionary();

	// open the socket
	_socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (_socket_fd == -1)
	{
		FATAL_ERR("Error while opening socket\n");
		throw std::runtime_error("socket");
	}

	// make the socket re-usable immediately after shutting down the webserv
	// then relaunching it

	const int enable = 1;
	if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) < 0)
	{
		FATAL_ERR("Error while setting up the socket\n");
		throw std::runtime_error("socket");
	}

	_in_fd.fd = _socket_fd;
	_in_fd.events = POLLIN;

	// Clear socket structure
	memset(&_sockaddr, 0, sizeof(_sockaddr));

	// Populate structure (IP)
	_sockaddr.sin_family = AF_INET;
	_sockaddr.sin_addr.s_addr = inet_addr(_server.getIP().c_str());
	_sockaddr.sin_port = htons(_server.getPort());

	// bind socket to given host:port
	if (bind(_socket_fd, (struct sockaddr *)(&_sockaddr), sizeof(sockaddr)) == -1)
	{
		FATAL_ERR("Error while binding socket\n");
		throw std::runtime_error("bind");
	}

	if (listen(_socket_fd, DEFAULT_MAX_CONNECTIONS) == -1)
	{
		FATAL_ERR("Error while listening to socket\n");
		throw std::runtime_error("listen");
	}

	_peer_addr_size = sizeof(_sockaddr);
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ServerEngine::~ServerEngine()
{
	if (_socket_fd != -1)
		close(_socket_fd);

	for (std::map<int, Connection>::iterator it = _aliveConnections.begin(); 
		it != _aliveConnections.end(); ++it)
	{
		if (it->second.req != NULL)
		{
			delete it->second.req;
			it->second.req = NULL;
		}
		if (it->second.resp != NULL)
		{
			delete it->second.resp;
			it->second.resp = NULL;
		}
		close(it->first);
	}
}

/*
** --------------------------------- STREAM IN / OUT ---------------------------------
*/

/*
** --------------------------------- METHODS ----------------------------------
*/

void ServerEngine::_getMethod()
{
	// return index page if a directory is requested as URL
	if (_req->getBody().type == "directory")
	{
		_resp->setStatusCode(SUCCESS_OK);
		_resp->setStatusMsg(err_dictionary.find(SUCCESS_OK)->second);
		_resp->setContentType("text/html");

		std::string path = _req->getTargetLocation()->getRoot();
		std::string body;
		if (_req->getTargetLocation()->isAutoindexed())
		{
			if (!path.empty() && path[path.size() - 1] == '/')
				path.append(_req->getHeader().URL.begin() + 1, _req->getHeader().URL.end());
			else
				path += _req->getHeader().URL;
			body = autoindexPageHtml(path, _req->getHeader().URL);
			_resp->setBody(body);
			_resp->setContentLength(body.size());
		}
		else
		{
			path += (_req->getTargetLocation()->getIndexPage()[0] == '/' ? "" : "/");
			path += _req->getTargetLocation()->getIndexPage();
			_resp->setIfstreamBodyHTML(path.c_str());
		}
	}
	// return a html page
	else if (_req->isValid() && _req->getBody().isMedia == false)
	{
		_resp->setStatusCode(SUCCESS_OK);
		_resp->setStatusMsg(err_dictionary.find(SUCCESS_OK)->second);
		_resp->setContentType(_req->getBody().type);

		std::string path = _req->getTargetLocation()->getRoot();
		path += (_req->getHeader().resource_path[0] == '/' ? "" : "/");
		path += _req->getHeader().resource_path;
		_resp->setIfstreamBodyHTML(path.c_str());
	}
	// return a media (image, video, etc.)
	else if (_req->isValid() && _req->getBody().isMedia)
	{
		_resp->setStatusCode(SUCCESS_OK);
		_resp->setStatusMsg(err_dictionary.find(SUCCESS_OK)->second);
		_resp->setContentType(_req->getBody().type);

		std::string path = _req->getTargetLocation()->getRoot();
		path += (_req->getHeader().resource_path[0] == '/' ? "" : "/");
		path += _req->getHeader().resource_path;

		_resp->setIfstreamBodyMedia(path.c_str());
	}
	else
	{
		_req->setIsRequestValid(false);
		_req->setError(SERVER_ERROR);
	}
}

void ServerEngine::_buildResponseOnRequest()
{
	// check if method is allowed
	if (!_req->isValid())
		goto error_case;
	if (!_req->getTargetLocation()->isAllowedMethod(_req->getHeader().method))
	{
		_req->setError(METHOD_NOT_ALLOWED);
		_req->setIsRequestValid(false);
	}
	else if (_req->getTargetLocation()->isRedirected())
	{
		int resp_code = _req->getTargetLocation()->getRedirection().first;

		_resp->setStatusCode(resp_code);
		_resp->setStatusMsg(err_dictionary.find(resp_code)->second);
		_resp->setRedirectionLocation(_req->getTargetLocation()->getRedirection().second);
	}
	else if (_req->getBody().type == "php" || _req->getBody().type == "py" || _req->getBody().type == "pl")
	{
		std::string cgi_output;

		CGIEngine cgi(_req, &_server);
		_resp->setFromCGI(true);
		cgi_output = cgi.exec();
		// DEBUG(cgi_output);
		_parse_CGI_output(cgi_output);
	}
	else if (_req->getHeader().method == "GET")
	{
		_getMethod();
	}
	else if (_req->getHeader().method == "POST")
	{
		_postMethod();
	}
	else if (_req->getHeader().method == "DELETE")
	{
		_deleteMethod();
	}

	// build response if error case
	if (!_req->isValid())
	{
	error_case:
		_resp->setStatusCode(_req->getError());
		_resp->setStatusMsg(err_dictionary.find(_req->getError())->second);
		_resp->setContentType("text/html");

		std::string		body;
		std::fstream	file;
		std::string		path;

		path = _server.getErrorPagePath(_req->getError());
		_resp->setIfstreamBodyHTML(path.c_str());
	}
}

void ServerEngine::_limit_request_size()
{
	if (_req->getBody().content.size() > _server.getClientBufferSize())
	{
		_req->getBody().content =
			_req->getBody().content.substr(0, _server.getClientBufferSize());
	}
}

void ServerEngine::_postMethod()
{
	std::fstream	file;
	std::string		path;
	std::string		upload_directory = "www/uploads/"; // where add files ? ( find the directory, finishing with a '/')

	// upload_directory = _req->getTargetLocation()->getRoot() + _req->getHeader().URL;
	DEBUG("upload_directory = " << upload_directory << '\n');

	if (_req->getHeader().content_type == "multipart/form-data")
	{
		std::string file_name;
		std::string line;
		std::string reqData = _req->getBody().content.substr(_req->getBody().content.find('\n') + 1);
		// std::vector<std::string>::iterator it = reqData.begin();
		line = reqData.substr(0, reqData.find('\n'));
		reqData = reqData.substr(reqData.find('\n') + 1);
		while (line != _req->getHeader().boundary + "--\r")
		{
			if (line.substr(0, 20) == "Content-Disposition:")
			{
				std::vector<std::string> line_items = split(line, " \r");
				file_name = line_items[2].substr(6);
				file_name = file_name.substr(0, file_name.rfind('\"'));
				// DEBUG("name = " << file_name << '\n');
			}
			else if (line == "\r")
			{
				// create file
				path = upload_directory + file_name;
				file.open(path.c_str(), std::ios::out);
				if (!file.is_open())
				{
					_resp->setStatusCode(SERVER_ERROR); // good error ??
					return;
				}
				// fill file
				line = reqData.substr(0, reqData.find('\n'));
				reqData = reqData.substr(reqData.find('\n') + 1);
				while (!reqData.empty())
				{
					std::string tmp = line;
					line = reqData.substr(0, reqData.find('\n'));
					reqData = reqData.substr(reqData.find('\n') + 1);
					if (line == _req->getHeader().boundary + "\r" || line == _req->getHeader().boundary + "--\r" || reqData.empty())
					{
						if (tmp.length() >= 1) // removing last '\r' caractere
							tmp.erase(tmp.end() - 1);
						file << tmp;
						break;
					}
					file << tmp;
					file << '\n';
				}
				file.close();
			}
			if (line != _req->getHeader().boundary + "--\r" && !reqData.empty())
			{
				line = reqData.substr(0, reqData.find('\n'));
				reqData = reqData.substr(reqData.find('\n') + 1);
			}
		}
	}

	_resp->setStatusCode(CREATED);
	_resp->setStatusMsg(err_dictionary.find(CREATED)->second);
	_resp->setContentType(_req->getBody().type);
	_resp->setContentLength(_req->getBody().length);
}

void ServerEngine::_deleteMethod()
{
	std::string	path;

	path = _req->getTargetLocation()->getRoot() + "/" + _req->getHeader().URL;
	if (unlink(path.c_str()) == -1)
	{
		_resp->setStatusCode(NOT_FOUND);
		_resp->setStatusMsg(err_dictionary.find(NOT_FOUND)->second);
	}
	else
	{
		_resp->setStatusCode(SUCCESS_OK);
		_resp->setStatusMsg(err_dictionary.find(SUCCESS_OK)->second);
	}
}

void ServerEngine::setGlobalConf(GlobalConfiguration *globalConf)
{
	_globalConf = globalConf;
}

void ServerEngine::_parse_CGI_output(std::string cgi_output)
{
	std::string					line;
	std::vector<std::string>	line_items;
	bool						status_set = false;
	std::istringstream			data(cgi_output);
	std::vector<std::string>	cookies;

	while (std::getline(data, line, '\n'))
	{
		if (line == "" || line == "\r")
			break;
		line_items = split(line, " \r");
		if (line_items.size() > 0 && line_items[0] == "Content-type:")
		{
			_resp->setContentType(line.c_str() + 14);
		}
		else if (line_items.size() > 2 && line_items[0] == "Status:")
		{
			_resp->setStatusCode(atoi(line_items[1].c_str()));
			_resp->setStatusMsg(err_dictionary.find(atoi(line_items[1].c_str()))->second);
			status_set = true;
		}
		else if (line_items.size() > 0 && line_items[0] == "Set-Cookie:")
		{
			cookies.push_back(line);
		}
		else if (line_items.size() > 0 && line_items[0] == "Expires:")
		{
			_resp->setExpires(line);
		}
		else if (line_items.size() > 0 && line_items[0] == "Cache-Control:")
		{
			_resp->setCacheControl(line);
		}
		else if (line_items.size() > 0 && line_items[0] == "Pragma:")
		{
			_resp->setPragma(line);
		}
	}

	if (!cookies.empty())
		_resp->setCookieLine(cookies);

	std::string cgi_output_body;
	while (std::getline(data, line, '\n'))
	{
		cgi_output_body += line;
		cgi_output_body += "\n";
	}
	// DEBUG(cgi_output_body);

	_resp->setBody(cgi_output_body);
	_resp->setContentLength(cgi_output_body.size());
	if (!status_set)
	{
		_resp->setStatusCode(SUCCESS_OK);
		_resp->setStatusMsg(err_dictionary.find(SUCCESS_OK)->second);
	}
}

void ServerEngine::_buildRequest()
{
	// for information only

	std::string req_str(_req->getRawData().begin(), _req->getRawData().end());
	std::cerr << "Request received:\n"
			  << BLUE_TXT << req_str << RESET_TXT << std::endl;

	// unchunk ?

	// store and parse the received request
	_req->parseData();

	if (!_req->isValid())
	{
		return;
	}

	// check if virtual server is used
	_virtual_server = _req->enableVirtualServer(_globalConf, _server);
	if (_virtual_server)
		_req->findLocation(*_virtual_server);
	else
		_req->findLocation(_server);

	if (!_req->isValid() || _req->identifyType() || _req->checkAccess())
	{
		return;
	}

	_req->extractBody();

	// shorten POST request body if it's overflowed
	if (_server.getClientBufferSize() >= 0)
		_limit_request_size();
}

void ServerEngine::stream_in(int poll_client_fd)
{
	int		client_fd;
	char	buffer[REQUEST_BUFFER_SIZE + 1] = {0};
	int		r;

	if (poll_client_fd == -1) // new connection
	{
		client_fd = accept(_socket_fd,
						   (struct sockaddr *)&_sockaddr,
						   &_peer_addr_size);

		if (client_fd == -1)
		{
			FATAL_ERR("Error while listening to socket\n");
			throw std::runtime_error("accept");
		}

		_globalConf->addClientFd(client_fd, POLLIN, this);
		fd_set_blocking(client_fd, 0);

		_req = new Request;
		_aliveConnections[client_fd] = Connection(_req, NULL);

		return;
	}
	else
	{
		_req = _aliveConnections[poll_client_fd].req;
		client_fd = poll_client_fd;
	}

	// Read from the connection

	r = recv(client_fd, buffer, REQUEST_BUFFER_SIZE, 0);
	// DEBUG("READ QTY: " << r << "\n\n");
	_globalConf->updateClientFd(client_fd, POLLIN | POLLOUT, this);
	if (r >= 0)
	{
		_req->getRawData().insert(_req->getRawData().end(), buffer, buffer + r);
		return; // we can read more
	}
	else if (r == -1)
	{
		perror("recv");
		_globalConf->updateClientFd(client_fd, POLLOUT, this);
		// nothing more to read
	}
	else if (r == 0)
	{
		_globalConf->updateClientFd(client_fd, POLLOUT, this);
		return; // nothing more to read
	}
}

int ServerEngine::stream_out(int client_fd)
{
	int still_alive = 0;
	std::string to_send;

	_req = _aliveConnections[client_fd].req;
	_globalConf->updateClientFd(client_fd, POLLOUT, this);

	// check if req exists /!!!

	if (_aliveConnections[client_fd].resp == NULL) // new connection
	{
		_buildRequest();
		_resp = new Response;
		_aliveConnections[client_fd].resp = _resp;
		_buildResponseOnRequest();
		if (_resp->isFromCGI() && _req->isValid())
			to_send = _resp->getCGIText();
		else if (_req->getTargetLocation() && _req->getTargetLocation()->isRedirected())
			to_send = _resp->getRedirText();
		else
		{
			to_send = _resp->getHeaderText() + '\n';
			if (_resp->getIfstreamBody().is_open()) // body = fd
				still_alive = 1;
			else // body = string
				to_send += _resp->getBody().content;
		}
	}
	else // old connection (fd)
	{
		_resp = _aliveConnections[client_fd].resp;

		std::string str;
		char c;

		while (str.length() < CHUNKED_RESPONSE_SIZE && _resp->getIfstreamBody().get(c))
			str += c;

		to_send = itohex(str.length()) + "\r\n" + str + "\r\n";
		if (str.length() < CHUNKED_RESPONSE_SIZE)
		{
			to_send += "0\r\n\r\n";
			_resp->getIfstreamBody().close();
		}
		else
			still_alive = 1;
	}

	//DEBUG("client_fd =\n" << client_fd << '\n' << "to_send =\n" << to_send << '\n');

	if (send(client_fd, to_send.c_str(), to_send.size(), MSG_NOSIGNAL) == -1)
		still_alive = 0; // clean
	if (still_alive)
		return still_alive;

	// Close the connection
	delete _resp;
	_resp = NULL;
	_aliveConnections[client_fd].resp = NULL;
	
	delete _req;
	_req = NULL;
	_aliveConnections[client_fd].req = NULL;
	
	kill_client(client_fd);
	return still_alive;
}

void ServerEngine::kill_client(int client_fd)
{
	if (_aliveConnections[client_fd].req != NULL)
	{
		delete _aliveConnections[client_fd].req;
		_aliveConnections[client_fd].req = NULL;
	}
	if (_aliveConnections[client_fd].resp != NULL)
	{
		delete _aliveConnections[client_fd].resp;
		_aliveConnections[client_fd].resp = NULL;
	}

	_aliveConnections.erase(client_fd);
	close(client_fd);
	_globalConf->eraseClientFd(client_fd);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

int ServerEngine::getSocketFd() const
{
	return _socket_fd;
}

struct pollfd *ServerEngine::getInFdPtr()
{
	return &_in_fd;
}

/* ************************************************************************** */
