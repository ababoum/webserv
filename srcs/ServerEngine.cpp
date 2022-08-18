/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerEngine.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 18:11:38 by mababou           #+#    #+#             */
/*   Updated: 2022/08/18 16:59:38 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ServerEngine.hpp"


void ServerEngine::init_dictionary()
{
	err_dictionary.insert(make_pair(200, "OK"));
	err_dictionary.insert(make_pair(400, "Bad Request"));
	err_dictionary.insert(make_pair(404, "Resource Not Found"));
	err_dictionary.insert(make_pair(500, "Internal Server Error"));
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

ServerEngine::ServerEngine(Server & server):_server(server)
{
	
	// init params
	_client_fd = -1;
	
	// open the socket
	_socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (_socket_fd == -1)
	{
		std::cerr << RED_TXT << "Error while opening socket\n" << RESET_TXT;
		throw std::runtime_error("socket");
	}
	
	_in_fd.fd = _socket_fd;
	_in_fd.events = POLLIN;
	_out_fd.fd = -1;

	// Clear socket structure
	memset(&_sockaddr, 0, sizeof(_sockaddr));
	
	// Populate structure
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
	close(_socket_fd);
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/


/*
** --------------------------------- METHODS ----------------------------------
*/

void	ServerEngine::_buildResponseOnRequest()
{
	if (!_req->getTargetLocation()->getCGI().empty())
	{
		CGIEngine cgi(_req);
		_resp->setFromCGI(true);
		_resp->setCGIText(cgi.exec());
		
		if (_req->isValid())
			return ;
	}
	else if (_req->isValid() && _req->getHeader().URL == "/")
	{
		_resp->setStatusCode(SUCCESS_OK);
		_resp->setStatusMsg("OK");
		_resp->setContentType("text/html");

		std::string body;
		std::string path = _req->getTargetLocation()->getRoot() + "/" + _req->getTargetLocation()->getIndexPage();
		std::fstream	file;
		
		file.open(path.c_str(), std::ios::in);
		for (std::string line; std::getline(file, line);)
		{
			body.append(line);
		}
		file.close();

		_resp->setBody(body);
		_resp->setContentLength(body.size());

		send(_client_fd, _resp->getText().c_str(), _resp->size(), 0);
	}
	else if (_req->isValid() && _req->getHeader().URL == "/favicon.ico")
	{
		_resp->setStatusCode(SUCCESS_OK);
		_resp->setStatusMsg("OK");
		_resp->setContentType("image/ico");

		std::vector<char> img_bytes = img_to_chars("www/favicon.ico");		
		std::string body(img_bytes.begin(), img_bytes.end());
		
		_resp->setBody(body);
		_resp->setContentLength(body.size());

		send(_client_fd, _resp->getText().c_str(), _resp->size(), 0);
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
			path += SSTR("" << _req->getError());
			path += ".html";
		}
		else
			path = _server.getErrorPagePath(_req->getError());
		
		body = htmlPath_to_string(path.c_str);

		_resp->setBody(body);
		_resp->setContentLength(body.size());
	}

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
	
	std::cout << "The request data was: " << \
		BLUE_TXT << request_data << RESET_TXT;
	
	_out_fd.fd = _client_fd;
	_out_fd.events = POLLOUT;

	// parse the request

	_req = new Request;
	_req->parseData(request_data);
	_req->findLocation(_server);
	
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

