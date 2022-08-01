/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 19:54:34 by mababou           #+#    #+#             */
/*   Updated: 2022/08/01 11:40:23 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"

int main(int ac, char **av)
{
	int socket_fd;
	int connection_fd;
	t_sockaddr_in sockaddr;
	socklen_t peer_addr_size;
	

	if (ac < 2)
	{
		std::cerr << "Webserv needs a configuration file to be launched\n"
			<< "usage: ./webserv [file.conf]" << std::endl;
		
		return (EXIT_FAILURE);
	}

	(void)av;
	
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1)
		handle_error("socket");
	
	// Clear structure
	ft_memset(&sockaddr, 0, sizeof(sockaddr));

	// Populate structure
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockaddr.sin_port = htons(9999);
                               
	if (bind(socket_fd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) == -1)
		handle_error("bind");

	if (listen(socket_fd, DEFAULT_MAX_CONNECTIONS) == -1)
		handle_error("listen");

	//  Now we can accept incoming connections one
    //           at a time using accept

	peer_addr_size = sizeof(sockaddr);

	while (1)
	{
		std::cout << "Waiting for a new connection...\n";
		
		connection_fd = accept(socket_fd, 
			(struct sockaddr*)&sockaddr,
			&peer_addr_size);
		
		if (connection_fd == -1)
			handle_error("accept");

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

	// close the socket
	close(socket_fd);

	return (EXIT_SUCCESS);
}