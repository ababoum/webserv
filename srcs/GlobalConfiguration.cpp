/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GlobalConfiguration.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 15:13:51 by mababou           #+#    #+#             */
/*   Updated: 2022/09/22 17:55:58 by mababou          ###   ########.fr       */
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
	for (size_t i = 0; i < _serverEngines.size(); ++i)
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
		_fds_ptr[*_serverEngines.back()->getInFdPtr()] = _serverEngines.back();
		_serverEngines.back()->setGlobalConf(this);
	}
}

void	GlobalConfiguration::dispatchStream(std::vector<struct pollfd> fds)
{
	static size_t			i = 0;
	size_t			threshold = _serversList.size();
	
	for (; i < fds.size(); ++i)
	{
		// DEBUG("i: " << i << '\n');
		if (fds[i].revents == POLLIN || fds[i].revents == (POLLIN | POLLOUT))
		{
			// DEBUG("FD_IN: " << fds[i].fd << '\n');
			// DEBUG("REVENTS: " << fds[i].revents << "\n\n");
			ServerEngine *_targetServ = _fds_ptr[fds[i]];
			if (i < threshold)
			{
				_targetServ->stream_in(-1);
			}
			else
			{
				_targetServ->stream_in(fds[i].fd);
			}
			++i;
			return ;
		}
		else if (fds[i].revents == POLLOUT)
		{
			// DEBUG("FD_OUT: " << fds[i].fd << '\n');
			// DEBUG("REVENTS: " << fds[i].revents << "\n\n");
			ServerEngine *_targetServ = _fds_ptr[fds[i]];
			if (_targetServ->stream_out(fds[i].fd))
				++i;
			i = (i >= fds.size()) ? 0 : i;
			return ;
		}
		else if (fds[i].revents & POLLERR || fds[i].revents & POLLNVAL)
		{
			ServerEngine *_targetServ = _fds_ptr[fds[i]];
			_targetServ->kill_client(fds[i].fd);
			return ;
		}
	}
	i = 0;
}

void	GlobalConfiguration::addClientFd(int fd, int flag_events, ServerEngine *serv_ptr)
{
	struct pollfd s_fd;

	s_fd.fd = fd;
	s_fd.events = flag_events;
	
	_fds_ptr[s_fd] = serv_ptr;
}

void	GlobalConfiguration::updateClientFd(int fd, int flag_events, ServerEngine *serv_ptr)
{
	struct pollfd s_fd;

	s_fd.fd = fd;
	s_fd.events = flag_events;

	eraseClientFd(fd);
	_fds_ptr[s_fd] = serv_ptr;
}

void	GlobalConfiguration::eraseClientFd(int fd)
{
	struct pollfd tmp;

	tmp.fd = fd;
	_fds_ptr.erase(tmp);
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

t_fds_map &	GlobalConfiguration::getFdsPtr()
{
	return _fds_ptr;	
}


/* ************************************************************************** */