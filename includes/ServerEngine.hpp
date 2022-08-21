/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerEngine.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 18:11:39 by mababou           #+#    #+#             */
/*   Updated: 2022/08/21 20:21:01 by mababou          ###   ########.fr       */
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
		typedef struct sockaddr_in			t_sockaddr_in;
		std::map<int, std::string>	err_dictionary;

	public:
		// ServerEngine(const ServerEngine & src);
		ServerEngine(Server & server);
		~ServerEngine();

	private:

		int				_socket_fd;
		t_sockaddr_in	_sockaddr;
		socklen_t		_peer_addr_size;
		Server &		_server;
		int				_client_fd;
		struct pollfd	_in_fd;
		struct pollfd	_out_fd;

		// request / response
		Request			*_req;
		Response		*_resp;

		void			_buildResponseOnRequest();
		void			_init_dictionary();

	public:

		void			stream_in();
		void			stream_out();
		int				getSocketFd() const;
		struct pollfd	* getInFdPtr();
		struct pollfd	* getOutFdPtr();

};

#endif /* **************************************************** SERVERENGINE_H */