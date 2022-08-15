/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerEngine.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tidurand <tidurand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 18:11:38 by mababou           #+#    #+#             */
/*   Updated: 2022/08/15 15:15:01 by tidurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ServerEngine.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ServerEngine::ServerEngine(Server & server):_server(server)
{
	
	// init params
	_client_fd = -1;
	
	// open the socket
	_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket_fd == -1)
	{
		std::cerr << RED_TXT << "Error while opening socket\n" << RESET_TXT;
		throw std::runtime_error("socket");
	}

	_in_fd.fd = _socket_fd;
	_in_fd.events = POLLIN;
	_out_fd.fd = -1;

	// Clear socket structure
	ft_memset(&_sockaddr, 0, sizeof(_sockaddr));
	
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
	// build response if error case



	// else
	// if (!_req->getTargetLocation()->getCGI().empty())
	// {
	// 	CGIEngine cgi(_req);
	// 	_resp->setFromCGI(true);
	// 	_resp->setCGIText(cgi.exec());
	// }
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
	char buffer[REQUEST_BUFFER_SIZE + 1] = {0};
	std::string		request_data;
	int r = 0;
	
	while ((r = recv(_client_fd, buffer, REQUEST_BUFFER_SIZE, MSG_DONTWAIT)) > 0)
	{
		sleep(1);
		std::cout << r << '\n';
		request_data.append(buffer);
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
		_resp->setStatusCode(SUCCESS_OK);
		_resp->setStatusMsg("OK");
		_resp->setContentType("text/html");

		std::string port_str = SSTR("" << _server.getPort() << "\n");
		std::string body;
		std::string path = _req->getTargetLocation()->getRoot() + "/" + _req->getTargetLocation()->getIndexPage();
		std::fstream	file;
		
		file.open(path, std::ios::in);
		for (std::string line; std::getline(file, line);)
		{
			body.append(line);
		}
		
		
		_resp->setBody(body);
		_resp->setContentLength(body.size());

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