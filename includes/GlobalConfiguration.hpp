/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GlobalConfiguration.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 15:13:51 by mababou           #+#    #+#             */
/*   Updated: 2022/09/07 16:05:46 by mababou          ###   ########.fr       */
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
		std::vector<struct pollfd *>	_fds_ptr;

	public:

		// methods & setters

		void	addServer(void);
		void	addServer(Server & server);
		void	setAllowedConnections(int nbConnectionsMax);
		void	startEngines();
		void	dispatchStream(std::vector<struct pollfd> fds);

		// accessors
		
		int								getAllowedConnections() const;
		std::vector<Server> & 			getServersList();
		std::vector<ServerEngine *> &	getEngines();
		std::vector<struct pollfd *> &	getFdsPtr();

};

#endif /* ********************************************* GLOBALCONFIGURATION_H */