/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GlobalConfiguration.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 15:13:51 by mababou           #+#    #+#             */
/*   Updated: 2022/08/01 20:42:15 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBALCONFIGURATION_HPP
# define GLOBALCONFIGURATION_HPP

# include "../includes/webserv.hpp"

class Server;

class GlobalConfiguration
{

	public:

		GlobalConfiguration();
		GlobalConfiguration( GlobalConfiguration const & src );
		~GlobalConfiguration();

		GlobalConfiguration &		operator=( GlobalConfiguration const & rhs );

	private:
		
		std::vector<Server>	_serversList;
		int					_nbAllowedConnections;

	public:

		// methods & setters

		void	addServer(void);
		void	addServer(Server & server);
		void	setAllowedConnections(int nbConnectionsMax);

		// accessors
		
		int		getAllowedConnections() const;
		std::vector<Server> & getServersList();

};

#endif /* ********************************************* GLOBALCONFIGURATION_H */