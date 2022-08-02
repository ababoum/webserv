/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationParser.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 15:08:25 by mababou           #+#    #+#             */
/*   Updated: 2022/08/02 18:51:29 by mababou          ###   ########.fr       */
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
	(void)line_nb;
	
	if (line_items.size() == 1 && line_items[0] == "}")
	{
		_context = "server";
		return ;
	}
}


void	ConfigurationParser::_parseServerLine(std::vector<std::string> & line_items, std::size_t line_nb)
{

	// end of scope
	if (line_items.size() == 1 && line_items[0] == "}")
	{
		_context = "main";
		_checkCurrentServerIntegrity(line_nb);
		return ;
	}

	// start of location scope
	else if (line_items.size() == 3 && line_items[0] == "location" && line_items[2] == "{")
	{
		_context = "location";
		Location *new_route = &(_globalConf.getServersList().back().addLocation());
		new_route->setPrefix(line_items[1]);
		_currentLocation = &(_currentServer->getRoutes().back());
		return ;
	}

	// check if ';' closes the line
	else if (line_items.back().at(line_items.back().size() - 1) != ';')
	{
		std::cerr << "Missing ';' at the end of line " << line_nb << '\n';
		throw syntax_error();
	}

	// fill port and host (IP)
	else if (line_items[0] == "listen" && line_items.size() == 2)
	{
		while(line_items.back().at(line_items.back().size() - 1) == ';')
			line_items.back().resize(line_items.back().size() - 1);

		_currentServer->setIP(split(line_items.back(), ':')[0]);
		_currentServer->setPort(split(line_items.back(), ':')[1]);
	}

	// fill server names
	else if (line_items[0] == "server_name" && line_items.size() >= 2)
	{
		for (std::size_t i = 1; i < line_items.size(); ++i)
			_currentServer->addName(line_items[i]);
	}

	// fill error pages
	else if (line_items[0] == "error_page" && line_items.size() == 3)
	{
		_currentServer->addErrorPage(
			std::atoi(line_items[1].c_str()),
			line_items[2]);
	}	
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
			_currentServer = &(_globalConf.getServersList().back());
		}
		else if (line_items.size() == 1 && line_items[0] == "}")
		{
			std::cerr << "Syntax error: extra '}' in the configuration file\n";
			throw syntax_error();
		}
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
	std::size_t		line_nb = 0;
	std::string 	currentLine;
	
	inputStream.open(_inputFilePath.c_str());

	// check file opening integrity
	if (inputStream.fail() || inputStream.eof() || inputStream.bad())
	{
		throw std::ios_base::failure("Error while opening configuration file\n");
	}

	// start parsing the file
	while (std::getline(inputStream, currentLine, '\n'))
	{
		std::vector<std::string>	line_items;
		
		++line_nb;
		line_items = split(currentLine, " \t");

		// empty line in the middle of the file
		if (line_items.empty())
			continue ;
		
		// skip commented lines
		else if (line_items[0][0] == '#')
			continue ;
		
		// erase end-of-line comment then parse the rest of the line
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
	}

	if (_context != "main")
	{
		std::cerr << "Syntax error: missing '}' in the configuration file\n";
		throw syntax_error();
	}
	
	inputStream.close();
}

void	ConfigurationParser::_checkCurrentServerIntegrity(std::size_t line_nb) const
{
	if (_currentServer->getPort() == -1)
	{
		std::cerr << "Error: server doesn't have a designated port: line " << line_nb << '\n';
		throw std::logic_error("port-less server\n");
	}
	if (_currentServer->getRoutes().empty())
	{
		std::cerr << "Error: server doesn't have any route: line " << line_nb << '\n';
		throw std::logic_error("route-less server\n");
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */