/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerEngine.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tidurand <tidurand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 18:11:39 by mababou           #+#    #+#             */
/*   Updated: 2022/08/08 12:13:20 by tidurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERENGINE_HPP
# define SERVERENGINE_HPP

# include "../includes/webserv.hpp"

class Server;
class Request;
class Response;

class ServerEngine
{
	public:
		typedef struct sockaddr_in t_sockaddr_in;

	public:

		ServerEngine(const Server & server);
		~ServerEngine();

	private:

		int				_socket_fd;
		t_sockaddr_in	_sockaddr;
		socklen_t		_peer_addr_size;
		const Server &	_server;
		int				_client_fd;
		struct pollfd	_in_fd;
		struct pollfd	_out_fd;

		// request / response
		// Request			*_req;
		Response		*_resp;


	public:

		void			stream_in();
		void			stream_out();
		int				getSocketFd() const;
		struct pollfd	* getInFdPtr();
		struct pollfd	* getOutFdPtr();

};

#endif /* **************************************************** SERVERENGINE_H */