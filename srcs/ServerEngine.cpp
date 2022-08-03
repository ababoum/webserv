/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerEngine.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 18:11:38 by mababou           #+#    #+#             */
/*   Updated: 2022/08/03 15:37:03 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ServerEngine.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ServerEngine::ServerEngine(const Server & server):_server(server)
{
	
	// init params
	_client_fd = -1;
	_pending_response = false;
	
	// open the socket
	_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket_fd == -1)
	{
		std::cerr << "Error while opening socket\n";
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
		std::cerr << "Error while binding socket\n";
		throw std::runtime_error("bind");
	}

	if (listen(_socket_fd, DEFAULT_MAX_CONNECTIONS) == -1)
	{
		std::cerr << "Error while listening to socket\n";
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

void	ServerEngine::stream_in()
{
	_client_fd = accept(_socket_fd, 
		(struct sockaddr*)&_sockaddr,
		&_peer_addr_size);
	
	if (_client_fd == -1)
	{
		std::cerr << "Error while listening to socket\n";
		throw std::runtime_error("accept");
	}

	// Read from the connection
	char buffer[1024] = {0};
	read(_client_fd, buffer, 1024);
	std::cout << "The message was: " << \
		BLUE_TXT << buffer << RESET_TXT;
	
	_pending_response = true;
	_out_fd.fd = _client_fd;
	_out_fd.events = POLLOUT;
}

void	ServerEngine::stream_out()
{
	// Send a message to the connection
	std::string response;
	std::string port_str = SSTR("" << _server.getPort() << "\n");
	std::string body("Hello world!\nI'm on port "); body.append(port_str);
	std::string body_size = SSTR("" << body.size());
	response.append("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: \n\n");
	response.insert(response.size() - 2, body_size);
	response.append(body);
	send(_client_fd, response.c_str(), response.size(), 0);

	// Close the connection
	close(_client_fd);
	_client_fd = -1;
	_out_fd.fd = -1;
	_pending_response = false;
}

bool	ServerEngine::responseIsPending() const
{
	return _pending_response;
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