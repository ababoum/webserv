/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 15:14:54 by mababou           #+#    #+#             */
/*   Updated: 2022/08/01 16:20:52 by mababou          ###   ########.fr       */
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

static std::vector<string> split(const std::string &str, char delim)
{
	std::size_t i = 0;
	std::vector<std::string> list;

	std::size_t pos = str.find(delim);

	while (pos != std::string::npos)
	{
		list.push_back(str.substr(i, pos - i));
		i = ++pos;
		pos = str.find(delim, pos);
	}

	list.push_back(str.substr(i, str.length()));

	return list;
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server() {}

Server::Server(const Server &src)
	: _name(src._name), _IP(src._IP), _port(src._port),
	  _allowedMethods(src._allowedMethods), _indexPage(src._indexPage),
	  _errorPages(src._errorPages), _clientBufferSize(src._clientBufferSize) {}

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
		_name = src._name;
		_IP = src._IP;
		_port = src._port;
		_allowedMethods = src._allowedMethods;
		_indexPage = src._indexPage;
		_autoindex = src._autoindex;
		_errorPages = src._errorPages;
		_clientBufferSize = src._clientBufferSize;
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
		throw std::invalid_argument("Server name cannot be empty\n")
	else
		_name = name;
}

void Server::setIP(std::string IP)
{
	if (!_isIPValid(IP))
		throw std::invalid_argument("IP address is not valid\n")
	else
		_IP = IP;
}

void Server::setPort(std::string port)
{
	if (!isNumber(port))
		throw std::invalid_argument("Port number is not valid\n")
	else
		_port = std::atoi(port);
}

void Server::addAllowedMethod(std::string method)
{
	if (method.compare("GET") && 
		method.compare("POST") && 
		method.compare("DELETE"))
		throw std::invalid_argument("Listed method is not managed (valid methods: GET, POST, DELETE)\n")
	
	_allowedMethods.insert(method);
}

void Server::setIndexPage(std::string indexPagePath)
{
	_indexPage = indexPagePath;	
}

void Server::setAutoindex(bool on_off)
{
	_autoindex = on_off;
}

void Server::addErrorPage(int error_code, std::string filePath)
{
	_errorPages.insert(std::make_pair(error_code, filePath));
}

void Server::setClientBufferSize(std::size_t buffer_max)
{
	_clientBufferSize = buffer_max;
}

// Methods

bool Server::isAllowedMethod(std::string method) const
{
	_allowedMethods::iterator find_result = _allowedMethods.find();
	if (find_result == _allowedMethods.end())
		return false;
	else
		return true;
}

// Private Helpers

bool Server::_isIPValid(std::string IP) const
{
	if (IP.compare("localhost") == 0)
		return true;

	if (IP.size() < 7) // smallest value being '0.0.0.0'
		return false;

	// split the string into tokens
	std::vector<std::string> list = split(ip, '.');

	// if the token size is not equal to four
	if (list.size() != 4)
	{
		return false;
	}

	// validate each token
	for (list::iterator it == list.begin(); it != list.end(); ++it)
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

std::string Server::getIndexPage() const
{
	return _indexPage;
}

bool Server::isAutoindexed() const
{
	return _autoindex;
}

std::string Server::getErrorPagePath(int error_code) const
{
	_errorPages::iterator it = _errorPages.find(error_code);
	
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