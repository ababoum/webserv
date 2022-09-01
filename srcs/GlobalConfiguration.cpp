/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GlobalConfiguration.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 15:13:51 by mababou           #+#    #+#             */
/*   Updated: 2022/09/01 14:45:46 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/GlobalConfiguration.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

GlobalConfiguration::GlobalConfiguration() {}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

GlobalConfiguration::~GlobalConfiguration()
{
	for (size_t i = 0; _serverEngines.size(); ++i)
	{
		delete _serverEngines[i];
	}
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/



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

void	GlobalConfiguration::startEngines()
{
	for (std::size_t i = 0; i < _serversList.size(); ++i)
	{
		_serverEngines.push_back(new ServerEngine(getServersList()[i]));
		_fds_ptr.push_back(_serverEngines.back()->getInFdPtr());
		_fds_ptr.push_back(_serverEngines.back()->getOutFdPtr());
		_serverEngines.back()->setGlobalConf(this);
	}
}

void	GlobalConfiguration::dispatchStream(std::vector<struct pollfd> fds)
{
	for (std::size_t i = 0; i < fds.size(); ++i)
	{
		if (i % 2 == 0 && fds[i].revents & POLLIN)
		{
			_serverEngines[i / 2]->stream_in();
		}
		else if (i % 2 == 1 && fds[i].revents & POLLOUT)
		{
			_serverEngines[i / 2]->stream_out();
		}
	}
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

std::vector<ServerEngine *> &		GlobalConfiguration::getEngines()
{
	return _serverEngines;
}

std::vector<struct pollfd *> &	GlobalConfiguration::getFdsPtr()
{
	return _fds_ptr;	
}


/* ************************************************************************** */