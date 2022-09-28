/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GlobalConfiguration.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 15:13:51 by mababou           #+#    #+#             */
/*   Updated: 2022/09/27 19:36:46 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBALCONFIGURATION_HPP
# define GLOBALCONFIGURATION_HPP

# include "../includes/webserv.hpp"

class Server;
class ServerEngine;

class GlobalConfiguration
{

	public:

		GlobalConfiguration();
		~GlobalConfiguration();

	private:
		
		std::vector<Server>				_serversList;
		std::vector<ServerEngine *>		_serverEngines;
		t_fds_map						_fds_ptr;

	public:

		// methods & setters

		void	addServer(void);
		void	addServer(Server & server);
		void	addClientFd(int fd, int flag_events, ServerEngine *serv_ptr);
		void	updateClientFd(int fd, int flag_events, ServerEngine *serv_ptr);
		void	eraseClientFd(int fd);
		void	startEngines();
		void	dispatchStream(std::vector<struct pollfd> fds);

		// accessors
		
		std::vector<Server> & 			getServersList();
		std::vector<ServerEngine *> &	getEngines();
		t_fds_map						& getFdsPtr();

};

#endif /* ********************************************* GLOBALCONFIGURATION_H */