/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerEngine.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tidurand <tidurand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 18:11:38 by mababou           #+#    #+#             */
/*   Updated: 2022/09/06 17:18:25 by tidurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ServerEngine.hpp"


void ServerEngine::_init_dictionary()
{
	err_dictionary.insert(std::make_pair(200, "OK"));
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


static int fd_set_blocking(int fd, int blocking) {
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

ServerEngine::ServerEngine(Server & server):_server(server), _virtual_server(0)
{
	// build error dictionary
	_init_dictionary();
	
	// init params
	_client_fd = -1;
	
	// open the socket
	_socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (_socket_fd == -1)
	{
		std::cerr << RED_TXT << "Error while opening socket\n" << RESET_TXT;
		throw std::runtime_error("socket");
	}

	// make the socket re-usable immediately after shutting down the webserv
	// then relaunching it

	const int enable = 1;
	if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    {
		std::cerr << RED_TXT << "Error while setting up the socket\n" << RESET_TXT;
		throw std::runtime_error("socket");
	}
	
	_in_fd.fd = _socket_fd;
	_in_fd.events = POLLIN;
	_out_fd.fd = -1;

	// Clear socket structure
	memset(&_sockaddr, 0, sizeof(_sockaddr));
	
	// Populate structure (IP)
	_sockaddr.sin_family = AF_INET;
	_sockaddr.sin_addr.s_addr = inet_addr(_server.getIP().c_str());
	_sockaddr.sin_port = htons(_server.getPort());
	

	// bind socket to given host:port
	if (bind(_socket_fd, (struct sockaddr*)(&_sockaddr), sizeof(sockaddr)) == -1)
	{
		std::cerr << RED_TXT << "Error while binding socket\n" << RESET_TXT;
		throw std::runtime_error("bind");
	}

	if (listen(_socket_fd, DEFAULT_MAX_CONNECTIONS) == -1)
	{
		std::cerr << RED_TXT << "Error while listening to socket\n" << RESET_TXT;
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
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/


/*
** --------------------------------- METHODS ----------------------------------
*/

void	ServerEngine::_getMethod()
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
				path.append(_req->getHeader().URL.begin() + 1,_req->getHeader().URL.end());
			else
				path+=_req->getHeader().URL;
			std::cerr << path << std::endl;
			body = autoindexPageHtml(path);
		}
		else
		{
			path += (_req->getTargetLocation()->getIndexPage()[0] == '/' ? "" : "/");
			path += _req->getTargetLocation()->getIndexPage();
			body = htmlPath_to_string(path.c_str());
		}
		_resp->setBody(body);
		_resp->setContentLength(body.size());
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
		std::string body = htmlPath_to_string(path.c_str());

		_resp->setBody(body);
		_resp->setContentLength(body.size());
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

		std::string body = media_to_string(path.c_str());
		
		_resp->setBody(body);
		_resp->setContentLength(body.size());
	}
	else
	{
		_req->setIsRequestValid(false);
		_req->setError(SERVER_ERROR);
	}
}

void	ServerEngine::_buildResponseOnRequest()
{
	// check if method is allowed
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
	else if (_req->getTargetLocation()->isAutoindexed())
	{
		_resp->setStatusCode(SUCCESS_OK);
		_resp->setStatusMsg(err_dictionary.find(SUCCESS_OK)->second);
		_resp->setContentType("text/html");

		std::string path = _req->getTargetLocation()->getRoot();
		std::string body;
		
		if (!path.empty() && path[path.size() - 1] == '/')
			path.append(_req->getHeader().URL.begin() + 1,_req->getHeader().URL.end());
		else
			path+=_req->getHeader().URL;
		std::cerr << path << std::endl;
		body = autoindexPageHtml(path);
		
		_resp->setBody(body);
		_resp->setContentLength(body.size());
	}
	else if (!_req->getTargetLocation()->getCGI().empty() || _req->getBody().type == "cgi")
	{
		CGIEngine cgi(_req, &_server);
		_resp->setFromCGI(true);
		_resp->setCGIText(cgi.exec());

	}
	else if (_req->getHeader().method == "GET")
	{
		_getMethod();
	}
		
	// build response if error case
	if (!_req->isValid())
	{
		_resp->setStatusCode(_req->getError());
		_resp->setStatusMsg(err_dictionary.find(_req->getError())->second);
		_resp->setContentType("text/html");

		std::string		body;
		std::fstream	file;
		std::string		path;
		
		if (_server.getErrorPagePath(_req->getError()) == "")
		{
			path = "www/error_pages/";
			path += int_to_string(_req->getError());
			path += ".html";
		}
		else
			path = _server.getErrorPagePath(_req->getError());
		
		body = htmlPath_to_string(path.c_str());

		_resp->setBody(body);
		_resp->setContentLength(body.size());
	}
}

void	ServerEngine::_limit_request_size(std::string & request)
{
	while (request.size() > _server.getClientBufferSize())
	{
		request.erase(request.end() - 1);
	}
}

void	ServerEngine::_formMethod(std::string postData)
{
	if (postData.empty())
		return;
	std::fstream	file;
	std::string		path;

	path = _req->getTargetLocation()->getRoot() + "/" +  _req->getHeader().URL;
	
	file.open(path.c_str(), std::ios::out);
	if (!file.is_open())
	{
		_resp->setStatusCode(SERVER_ERROR);
		return;
	}
	file << postData;
	file.close();
	_resp->setStatusCode(CREATED);
}

void	ServerEngine::_deleteMethod()
{
	std::string		path;

	path = _req->getTargetLocation()->getRoot() + "/" +  _req->getHeader().URL;
	if (unlink(path.c_str()) == -1)
		_resp->setStatusCode(SERVER_ERROR);
	else
		_resp->setStatusCode(SUCCESS_OK);
}

void	ServerEngine::setGlobalConf(GlobalConfiguration *globalConf)
{
	_globalConf = globalConf;
}


void	ServerEngine::stream_in()
{
	_client_fd = accept(_socket_fd, 
		(struct sockaddr*)&_sockaddr,
		&_peer_addr_size);
	
	if (_client_fd == -1)
	{
		std::cerr << RED_TXT << "Error while listening to socket\n" << RESET_TXT;
		throw std::runtime_error("accept");
	}

	// Read from the connection
	char 			buffer[REQUEST_BUFFER_SIZE + 1] = {0};
	std::string		request_data;
	int r;
	
	readloop:
	r = read(_client_fd, buffer, REQUEST_BUFFER_SIZE);
	if (r > 0)
	{
		request_data.append(buffer);
		memset(buffer, 0, REQUEST_BUFFER_SIZE);
		fd_set_blocking(_client_fd, 0);
		goto readloop;
	}
	
	// this should be corrected and moved to appropriate place (after request is built)
	if (_server.getClientBufferSize() > 0)
		_limit_request_size(request_data);
	
	std::cout << "The request data was: " << \
		BLUE_TXT << request_data << RESET_TXT;
	
	_out_fd.fd = _client_fd;
	_out_fd.events = POLLOUT;

	// parse the request

	_req = new Request;
	_req->parseData(request_data);
	
	// check if virtual server is used
	_virtual_server = _req->enableVirtualServer(_globalConf, _server);
	if (_virtual_server)
		_req->findLocation(*_virtual_server); 
	else
		_req->findLocation(_server);

	_req->checkAccess();
	_req->identifyType();
	_req->getPostData(request_data);
	
}

void	ServerEngine::stream_out()
{
	// Send a response to the connection

	_resp = new Response;

	_buildResponseOnRequest();
	
	if (_resp->isFromCGI())
	{
		send(_client_fd, _resp->getCGIText().c_str(), _resp->size(), 0);
	}
	else if (_req->getTargetLocation()->isRedirected())
	{
		send(_client_fd, _resp->getRedirText().c_str(), _resp->size(), 0);
	}
	else
	{
		send(_client_fd, _resp->getText().c_str(), _resp->size(), 0);
	}
	
	delete _resp;
	delete _req;
	// Close the connection
	close(_client_fd);
	_client_fd = -1;
	_out_fd.fd = -1;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

int		ServerEngine::getSocketFd() const
{
	return _socket_fd;
}

struct pollfd	* ServerEngine::getInFdPtr()
{
	return &_in_fd;
}

struct pollfd	* ServerEngine::getOutFdPtr()
{
	return &_out_fd;
}

/* ************************************************************************** */

