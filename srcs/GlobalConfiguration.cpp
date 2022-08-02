/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GlobalConfiguration.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 15:13:51 by mababou           #+#    #+#             */
/*   Updated: 2022/08/02 14:18:44 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/GlobalConfiguration.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

GlobalConfiguration::GlobalConfiguration() {}

GlobalConfiguration::GlobalConfiguration( const GlobalConfiguration & src )
{
	*this = src;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

GlobalConfiguration::~GlobalConfiguration() {}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

GlobalConfiguration &	GlobalConfiguration::operator=( GlobalConfiguration const & rhs )
{
	if ( this != &rhs )
	{
		_serversList = rhs._serversList;
		_nbAllowedConnections = rhs._nbAllowedConnections;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void	GlobalConfiguration::addServer(Server & server)
{
	_serversList.push_back(server);
}

void	GlobalConfiguration::addServer(void)
{
	_serversList.push_back(Server());
}

void	GlobalConfiguration::setAllowedConnections(int nbConnectionsMax)
{
	_nbAllowedConnections = nbConnectionsMax;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

int		GlobalConfiguration::getAllowedConnections() const
{
	return _nbAllowedConnections;
}

std::vector<Server> & GlobalConfiguration::getServersList()
{
	return _serversList;
}

/* ************************************************************************** */