/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 19:13:53 by mababou           #+#    #+#             */
/*   Updated: 2022/08/08 15:25:43 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Location.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Location::Location()
{
}

Location::Location( const Location & src )
{
	*this = src;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Location::~Location()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Location &				Location::operator=( Location const & rhs )
{
	if ( this != &rhs )
	{		
		_prefix = rhs._prefix;
		_root = rhs._root;
		_allowedMethods = rhs._allowedMethods;
		_indexPage = rhs._indexPage;
		_autoindex = rhs._autoindex;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

// Setters

void Location::setPrefix(std::string prefix)
{
	_prefix = prefix;
}

void Location::setRoot(std::string path)
{
	_root = path;
}

void Location::addAllowedMethod(std::string method)
{
	if (method.compare("GET") && 
		method.compare("POST") && 
		method.compare("DELETE"))
		throw std::invalid_argument("Listed method is not managed (valid methods: GET, POST, DELETE)");
	
	_allowedMethods.insert(method);
}

void Location::setIndexPage(std::string indexPagePath)
{
	_indexPage = indexPagePath;	
}

void Location::setAutoindex(bool on_off)
{
	_autoindex = on_off;
}

// Methods

bool Location::isAllowedMethod(std::string method) const
{
	std::set<std::string>::iterator find_result = _allowedMethods.find(method);
	if (find_result == _allowedMethods.end())
		return false;
	else
		return true;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string Location::getPrefix() const
{
	return _prefix;
}

std::string Location::getIndexPage() const
{
	return _indexPage;
}

bool Location::isAutoindexed() const
{
	return _autoindex;
}


/* ************************************************************************** */