/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerEngine.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 18:11:39 by mababou           #+#    #+#             */
/*   Updated: 2022/09/21 15:31:25 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERENGINE_HPP
# define SERVERENGINE_HPP

# include "../includes/webserv.hpp"

class Server;
class Request;
class Response;

struct Connection
{
	Request		*req;
	Response	*resp;

	Connection(Request *req_input = 0, Response *resp_input = 0):
		req(req_input), resp(resp_input) {}
};


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

		int							_socket_fd;
		struct pollfd				_in_fd;
		t_sockaddr_in				_sockaddr;
		socklen_t					_peer_addr_size;
		Server &					_server;
		Server 						*_virtual_server;
		GlobalConfiguration			*_globalConf;
		std::map<int, Connection>	_aliveConnections;

		// request / response
		Request			*_req;
		Response		*_resp;

		void			_buildResponseOnRequest();
		void			_getMethod();
		void			_init_dictionary();
		void			_limit_request_size();
		void			_parse_CGI_output(std::string cgi_output);
		void			_postMethod(std::string postData);
		void			_deleteMethod();

	public:

		void			setGlobalConf(GlobalConfiguration *globalConf);
		void			stream_in();
		int				stream_out(int client_fd);
		int				getSocketFd() const;
		struct pollfd	* getInFdPtr();

};

#endif /* **************************************************** SERVERENGINE_H */