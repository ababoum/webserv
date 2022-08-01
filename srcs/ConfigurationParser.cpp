/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationParser.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 15:08:25 by mababou           #+#    #+#             */
/*   Updated: 2022/08/01 21:05:55 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ConfigurationParser.hpp"


static std::size_t find_comment(const std::vector<std::string> & line_items)
{
	std::size_t i;
	
	for (i = 0; i < line_items.size(); ++i)
	{
		if (line_items[i][0] == '#')
			break ;	
	}
	return i;
}


/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/


// Parameterized constructor
ConfigurationParser::ConfigurationParser(std::string inputFilePath, GlobalConfiguration & globalConf)
	: _inputFilePath(inputFilePath), _globalConf(globalConf), _context("main")
{
	_parseFile();
}

ConfigurationParser::ConfigurationParser( const ConfigurationParser & src )
	: _inputFilePath(src._inputFilePath), _globalConf(src._globalConf), _context("main") {}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ConfigurationParser::~ConfigurationParser() {}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

ConfigurationParser &	ConfigurationParser::operator=( ConfigurationParser const & rhs )
{
	if ( this != &rhs )
	{
		_inputFilePath = rhs._inputFilePath;
		_globalConf = rhs._globalConf;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void	ConfigurationParser::_parseLocationLine(std::vector<std::string> & line_items, std::size_t line_nb)
{
	(void)line_items;
	(void)line_nb;
}


void	ConfigurationParser::_parseServerLine(std::vector<std::string> & line_items, std::size_t line_nb)
{
	(void)line_items;
	(void)line_nb;
}


void	ConfigurationParser::_parseLine(std::vector<std::string> & line_items, std::size_t line_nb)
{
	// parse the line until its end (comment preliminary erased)
	
	if (_context == "server")
		_parseServerLine(line_items, line_nb);

	else if (_context == "location")
		_parseLocationLine(line_items, line_nb);
	
	else // main context
	{
		if (line_items.size() == 2 && line_items[0] == "server" && line_items[1] == "{")
		{
			_context = "server";
			_globalConf.addServer();
		}
		
		else if (line_items.size() == 3 && line_items[0] == "location" && line_items[2] == "{")
		{
			_context = "location";
			Location *new_route = &(_globalConf.getServersList().back().addLocation());
			new_route->setPrefix(line_items[1]);
		}
		else if (line_items.size() == 1 && line_items[0] == "}" && _context == "location")
			_context = "server";
		else if (line_items.size() == 1 && line_items[0] == "}" && _context == "server")
			_context = "main";
		else
		{
			std::stringstream ss;
			ss << line_nb;
			std::string line_nb_str;
			ss >> line_nb_str;
			throw parsing_error(line_nb_str.c_str());
		}
	}
}

void	ConfigurationParser::_parseFile()
{
	std::ifstream	inputStream;
	std::size_t	line_nb = 1;
	
	inputStream.open(_inputFilePath.c_str(), std::ifstream::in);

	// check file opening integrity
	if (inputStream.fail() || inputStream.eof() || inputStream.bad())
	{
		throw std::ios_base::failure("Error while opening configuration file\n");
	}

	// start parsing the file
	while (!inputStream.eof())
	{
		std::string 				currentLine;
		std::vector<std::string>	line_items;
		
		std::getline(inputStream, currentLine, '\n');
		line_items = split(currentLine, " \t");

		// empty line in the middle of the file
		if (line_items.empty())
			continue ;
		
		// skip commented lines
		else if (line_items[0][0] == '#')
			continue ;
		
		// erase end-of-line comment
		else
		{
			std::size_t comment_index = find_comment(line_items);
			if (comment_index != line_items.size())
			{
				for (std::size_t i = comment_index; i < line_items.size(); ++i)
					line_items.pop_back();
			}
			_parseLine(line_items, line_nb);
		}
		
		++line_nb;
	}
	
	if (_context != "main")
		throw syntax_error("Syntax error: missing '}' in the configuration file\n");
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */