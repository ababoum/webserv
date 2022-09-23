/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationParser.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 15:08:25 by mababou           #+#    #+#             */
/*   Updated: 2022/09/23 18:37:50 by mababou          ###   ########.fr       */
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
	if (line_items.size() == 1 && line_items[0] == "}")
	{
		_context = "server";
		_checkCurrentLocationIntegrity(line_nb);
		_solveCurrentLocationIntegrity();
		return ;
	}

	// check if ';' closes the line
	else if (line_items.back().at(line_items.back().size() - 1) != ';')
	{
		FATAL_ERR("Missing ';' at the end of line " << line_nb << '\n');
		throw syntax_error("syntax error");
	}

	// erase ';' at the end of the line
	while(line_items.back().size() >= 1 && line_items.back().at(line_items.back().size() - 1) == ';')
		line_items.back().erase(line_items.back().end() - 1);

	// fill root
	if (line_items[0] == "root" && line_items.size() == 2)
	{
		_currentLocation->setRoot(line_items[1]);
	}

	// fill allowed methods
	else if (line_items[0] == "methods" && line_items.size() >= 2)
	{
		for (std::size_t i = 1; i < line_items.size(); ++i)
			_currentLocation->addAllowedMethod(line_items[i]);
	}

	// fill autoindex
	else if (line_items[0] == "autoindex" && line_items.size() == 2)
	{
		_currentLocation->setAutoindex(line_items[1]);
	}

	// fill indexPage
	else if (line_items[0] == "index" && line_items.size() == 2)
	{
		_currentLocation->setIndexPage(line_items[1]);
	}

	// HTTP redirection
	else if (line_items[0] == "return" && line_items.size() == 3)
	{
		if (!is_digit(line_items[1]))
		{
			FATAL_ERR("Return code should be an integer at line " << line_nb << '\n');
			throw syntax_error("invalid http redirection directive");
		}
		_currentLocation->setRedirection(std::atoi(line_items[1].c_str()), line_items[2]);
	}

	// else: parsing error
	else
	{
		FATAL_ERR("Parsing error in line " << line_nb << '\n');
		throw parsing_error("invalid line");
	}
}


void	ConfigurationParser::_parseServerLine(std::vector<std::string> & line_items, std::size_t line_nb)
{
	// end of scope
	if (line_items.size() == 1 && line_items[0] == "}")
	{
		_context = "main";
		_checkCurrentServerIntegrity(line_nb);
		_currentServer->completeErrorPages();
		return ;
	}

	// start of location scope
	else if (line_items.size() == 3 && line_items[0] == "location" && line_items[2] == "{")
	{
		_context = "location";
		Location *new_route = &(_currentServer->addLocation());
		new_route->setPrefix(line_items[1]);
		
		// make sure that the prefix ends with '/' if it's a named folder
		if (line_items[1][line_items[1].size() - 1] != '/' && line_items[1].find('*') == std::string::npos && line_items[1].find('.') == std::string::npos)
		{

			line_items[1].append("/");
			new_route->setPrefix(line_items[1]);
		}
		_currentLocation = new_route;
		return ;
	}

	// check if ';' closes the line
	else if (line_items.back().at(line_items.back().size() - 1) != ';')
	{
		FATAL_ERR("Missing ';' at the end of line " << line_nb << '\n');
		throw syntax_error("syntax error");
	}

	// erase ';' at the end of the line
	while(line_items.back().size() >= 1 && line_items.back().at(line_items.back().size() - 1) == ';')
		line_items.back().erase(line_items.back().end() - 1);

	// fill port and host (IP)
	if (line_items[0] == "listen" && line_items.size() == 2)
	{
		std::vector<std::string> host_items = split(line_items.back(), ':');
		
		if (host_items.size() != 2)
		{
			FATAL_ERR("Invalid IP/Port in line " << line_nb << '\n');
			throw syntax_error("invalid server directive");
		}
		_currentServer->setIP(host_items[0]);
		_currentServer->setPort(host_items[1]);
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

	// fill maxBody size
	else if (line_items[0] == "client_max_body_size" && line_items.size() == 2)
	{
		_currentServer->setClientBufferSize(
			std::atoi(line_items[1].c_str()));
	}

	// else: parsing error
	else
	{
		FATAL_ERR("Parsing error in line " << line_nb << '\n');
		throw parsing_error("invalid line");
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
			FATAL_ERR("Syntax error: extra '}' in the configuration file\n");
			throw syntax_error("misplaced symbol");
		}
		else
		{
			FATAL_ERR("Parsing error in line " << line_nb << '\n');
			throw parsing_error("invalid line");
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
	// if (inputStream.fail() || inputStream.eof() || inputStream.bad())
	if (!inputStream)
	{
		throw std::ios_base::failure("Error while opening configuration file");
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
			while (comment_index != line_items.size())
				line_items.pop_back();
			_parseLine(line_items, line_nb);
		}
	}

	if (_context != "main")
	{
		FATAL_ERR("Syntax error: missing '}' in the configuration file\n");
		throw syntax_error("file syntax error");
	}

	if (_globalConf.getServersList().empty())
	{
		FATAL_ERR("No server detected in configuration file" << '\n');
		throw parsing_error("invalid file");
	}

	_checkServerDuplicate();
	inputStream.close();
}

void	ConfigurationParser::_checkServerDuplicate()
{
	for (std::size_t i = 0; i < _globalConf.getServersList().size() - 1; ++i)
	{
		for (std::size_t j = i + 1; j < _globalConf.getServersList().size(); ++j)
		{
			if (_globalConf.getServersList()[i].getIP() == _globalConf.getServersList()[j].getIP() &&
				_globalConf.getServersList()[i].getPort() == _globalConf.getServersList()[j].getPort())
				_globalConf.getServersList().erase(_globalConf.getServersList().begin() + j--);
		}
	}
}

void	ConfigurationParser::_checkCurrentServerIntegrity(std::size_t line_nb) const
{
	if (_currentServer->getPort() == -1)
	{
		FATAL_ERR("Error: server doesn't have a designated port: line " << line_nb << '\n');
		throw std::logic_error("port-less server");
	}
	if (_currentServer->getRoutes().empty())
	{
		FATAL_ERR("Error: server doesn't have any route: line " << line_nb << '\n');
		throw std::logic_error("route-less server");
	}
}

void	ConfigurationParser::_checkCurrentLocationIntegrity(std::size_t line_nb) const
{
	struct stat sb;
	
	if (_currentLocation->getRoot().empty())
	{
		FATAL_ERR("Error: location doesn't have a root: line " << line_nb << '\n');
		throw std::logic_error("root-less location");
	}
	else if (!(stat(_currentLocation->getRoot().c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)))
	{
		FATAL_ERR("Error: Root not valid: line " << line_nb << '\n');
		throw std::logic_error("root not valid");
	}
	

	if (_currentLocation->getIndexPage().empty() && !_currentLocation->isAutoindexed()
		&& is_folder_formatted(_currentLocation->getPrefix()))
	{
		FATAL_ERR("Error: location doesn't have an index page: line " << line_nb << '\n');
		throw std::logic_error("index-less location");
	}
}


void	ConfigurationParser::_solveCurrentLocationIntegrity()
{
	if (_currentLocation->getAllowedMethods().empty())
	{
		_currentLocation->addAllowedMethod("GET");
		_currentLocation->addAllowedMethod("POST");
		_currentLocation->addAllowedMethod("DELETE");
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */