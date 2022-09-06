/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerEngine.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tidurand <tidurand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 18:11:39 by mababou           #+#    #+#             */
/*   Updated: 2022/09/06 17:18:19 by tidurand         ###   ########.fr       */
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
		typedef struct sockaddr_in		t_sockaddr_in;

	public:
		std::map<int, std::string>		err_dictionary;

	public:
		// ServerEngine(const ServerEngine & src);
		ServerEngine(Server & server);
		~ServerEngine();

	private:

		int						_socket_fd;
		t_sockaddr_in			_sockaddr;
		socklen_t				_peer_addr_size;
		Server &				_server;
		Server 					*_virtual_server;
		int						_client_fd;
		struct pollfd			_in_fd;
		struct pollfd			_out_fd;
		GlobalConfiguration		*_globalConf;

		// request / response
		Request			*_req;
		Response		*_resp;

		void			_buildResponseOnRequest();
		void			_getMethod();
		void			_init_dictionary();
		void			_limit_request_size(std::string & request);
		void			_formMethod(std::string postData);
		void			_deleteMethod();

	public:

		void			setGlobalConf(GlobalConfiguration *globalConf);
		void			stream_in();
		void			stream_out();
		int				getSocketFd() const;
		struct pollfd	* getInFdPtr();
		struct pollfd	* getOutFdPtr();

};

#endif /* **************************************************** SERVERENGINE_H */