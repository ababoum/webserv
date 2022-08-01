/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationParser.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 15:08:25 by mababou           #+#    #+#             */
/*   Updated: 2022/08/01 15:13:40 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ConfigurationParser.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ConfigurationParser::ConfigurationParser(): _inputFilePath() {}


// Parameterized constructor
ConfigurationParser::ConfigurationParser(std::string inputFilePath)
	: _inputFilePath(inputFilePath) {}

ConfigurationParser::ConfigurationParser( const ConfigurationParser & src )
	:_inputFilePath(src._inputFilePath) {}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ConfigurationParser::~ConfigurationParser() {}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

ConfigurationParser &				ConfigurationParser::operator=( ConfigurationParser const & rhs )
{
	if ( this != &rhs )
	{
		_inputFilePath = rhs._inputFilePath;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */