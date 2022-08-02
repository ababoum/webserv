/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerEngine.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 18:11:39 by mababou           #+#    #+#             */
/*   Updated: 2022/08/02 18:41:20 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERENGINE_HPP
# define SERVERENGINE_HPP

# include "../includes/webserv.hpp"

class Server;

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

	public:

		void	run();
};

#endif /* **************************************************** SERVERENGINE_H */