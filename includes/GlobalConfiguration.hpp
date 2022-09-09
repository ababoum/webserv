/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GlobalConfiguration.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 15:13:51 by mababou           #+#    #+#             */
/*   Updated: 2022/09/09 17:44:24 by mababou          ###   ########.fr       */
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
		int								_nbAllowedConnections;
		t_fds_map						_fds_ptr;

	public:

		// methods & setters

		void	addServer(void);
		void	addServer(Server & server);
		void	addClientFd(int fd, int flag_events, ServerEngine *serv_ptr);
		void	eraseClientFd(int fd);
		void	setAllowedConnections(int nbConnectionsMax);
		void	startEngines();
		void	dispatchStream(std::vector<struct pollfd> fds);

		// accessors
		
		int								getAllowedConnections() const;
		std::vector<Server> & 			getServersList();
		std::vector<ServerEngine *> &	getEngines();
		t_fds_map						& getFdsPtr();

};

#endif /* ********************************************* GLOBALCONFIGURATION_H */