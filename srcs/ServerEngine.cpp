/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerEngine.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 18:11:38 by mababou           #+#    #+#             */
/*   Updated: 2022/08/02 18:46:31 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ServerEngine.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ServerEngine::ServerEngine(const Server & Server)
{
	// open the socket
	_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket_fd == -1)
	{
		std::cerr << "Error while opening socket\n";
		throw std::runtime_error("socket");
	}

	// Clear socket structure
	ft_memset(&_sockaddr, 0, sizeof(_sockaddr));
	
	// Populate structure
	_sockaddr.sin_family = AF_INET;
	_sockaddr.sin_addr.s_addr = inet_addr(Server.getIP().c_str());
	_sockaddr.sin_port = htons(Server.getPort());
	

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

void	ServerEngine::run()
{
	while (1)
	{
		int connection_fd;
		
		std::cout << "Waiting for a new connection...\n";
		
		connection_fd = accept(_socket_fd, 
			(struct sockaddr*)&_sockaddr,
			&_peer_addr_size);
		
		if (connection_fd == -1)
		{
			std::cerr << "Error while listening to socket\n";
			throw std::runtime_error("accept");
		}

		// Read from the connection
		char buffer[1024] = {0};
		read(connection_fd, buffer, 1024);
		std::cout << "The message was: " << \
			BLUE_TXT << buffer << RESET_TXT;

		// Send a message to the connection
		std::string response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
		send(connection_fd, response.c_str(), response.size(), 0);

		// Close the connection
		close(connection_fd);
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */