/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 15:14:54 by mababou           #+#    #+#             */
/*   Updated: 2022/08/01 21:11:43 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

/*
** --------------------------- EXTERNAL HELPER FUNCTIONS ----------------------
*/

// check if a given string is a numeric string or not
static bool isNumber(const std::string &str)
{
	return !str.empty() &&
		   (str.find_first_not_of("0123456789") == std::string::npos);
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server() {}

Server::Server(const Server &src)
{
	*this = src;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Server::~Server() {}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Server &Server::operator=(Server const &rhs)
{
	if (this != &rhs)
	{
		_name = rhs._name;
		_IP = rhs._IP;
		_port = rhs._port;
		_errorPages = rhs._errorPages;
		_clientBufferSize = rhs._clientBufferSize;
		_routes = rhs._routes;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

// Setters

void Server::setName(std::string name)
{
	if (name.empty())
		throw std::invalid_argument("Server name cannot be empty\n");
	else
		_name = name;
}

void Server::setIP(std::string IP)
{
	if (!_isIPValid(IP))
		throw std::invalid_argument("IP address is not valid\n");
	else
		_IP = IP;
}

void Server::setPort(std::string port)
{
	if (!isNumber(port))
		throw std::invalid_argument("Port number is not valid\n");
	else
		_port = std::atoi(port.c_str());
}

void Server::addErrorPage(int error_code, std::string filePath)
{
	_errorPages.insert(std::make_pair(error_code, filePath));
}

void Server::setClientBufferSize(std::size_t buffer_max)
{
	_clientBufferSize = buffer_max;
}

Location & Server::addLocation()
{
	_routes.push_back(Location());
	return _routes.back();
}

// Private Helpers

bool Server::_isIPValid(std::string IP) const
{
	if (IP.compare("localhost") == 0)
		return true;

	if (IP.size() < 7) // smallest value being '0.0.0.0'
		return false;

	// split the string into tokens
	std::vector<std::string> list = split(IP, '.');

	// if the token size is not equal to four
	if (list.size() != 4)
	{
		return false;
	}

	// validate each token
	for (std::vector<std::string>::iterator it = list.begin(); it != list.end(); ++it)
	{
		// verify that the string is a number or not, and the numbers
		// are in the valid range
		if (!isNumber(*it) ||
			std::atoi(it->c_str()) > 255 ||
			std::atoi(it->c_str()) < 0)
		{
			return false;
		}
	}
	return true;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string Server::getName() const
{
	return _name;
}

std::string Server::getIP() const
{
	return _IP;
}

int Server::getPort() const
{
	return _port;
}

std::string Server::getErrorPagePath(int error_code) const
{
	std::map<int, std::string>::const_iterator it = _errorPages.find(error_code);
	
	if (it == _errorPages.end())
		return "";
	else
		return it->second;
}

std::size_t Server::getClientBufferSize() const
{
	return _clientBufferSize;
}

/* ************************************************************************** */