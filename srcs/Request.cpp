/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tidurand <tidurand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 12:11:55 by mababou           #+#    #+#             */
/*   Updated: 2022/09/19 13:30:57 by tidurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Request.hpp"


// GOOGLE CHROME REQUEST EXAMPLES

// GET / HTTP/1.1
// Host: localhost:9999
// Connection: keep-alive
// sec-ch-ua: " Not A;Brand";v="99", "Chromium";v="102", "Google Chrome";v="102"
// sec-ch-ua-mobile: ?0
// sec-ch-ua-platform: "Linux"
// DNT: 1
// Upgrade-Insecure-Requests: 1
// User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/102.0.0.0 Safari/537.36
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
// Sec-Fetch-Site: none
// Sec-Fetch-Mode: navigate
// Sec-Fetch-User: ?1
// Sec-Fetch-Dest: document
// Accept-Encoding: gzip, deflate, br
// Accept-Language: en-US,en;q=0.9,fr;q=0.8



/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Request::Request(): _validRequest(true), _error(NO_ERROR),_targetLocation(NULL)
{

}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Request::~Request()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/



/*
** --------------------------------- METHODS ----------------------------------
*/

void	Request::parseData(std::string requestData)
{
	std::istringstream 	data(requestData);
	std::string			line;
	size_t				line_index = 1;

	if (requestData.empty())
	{
		_error = BAD_REQUEST;
		_validRequest = false;
		return ;
	}
	while(std::getline(data, line, '\n'))
	{
		std::vector<std::string> line_items = split(line, " \r");
		// Method line
		if (line_index == 1)
		{
			if (line_items.size() != 3)
			{
				_error = BAD_REQUEST;
				_validRequest = false;
				return ;
			}
			else if (line_items[0] != "GET" && line_items[0] != "POST" && line_items[0] != "DELETE")
			{
				_error = METHOD_NOT_ALLOWED;
				_validRequest = false;
				return ;
			}
			else
			{
				_header.method = line_items[0];
				_header.URL = line_items[1];
				_parseURL();	
			}
		}
		// Host line
		else if (line_items.size() > 1 && line_items[0] == "Host:")
		{
			_header.host = line_items[1];
		}
		// User-Agent line
		else if (line_items.size() > 1 && line_items[0] == "User-Agent:")
		{
			_header.user_agent = line.substr(12, line.length() - 13);
		}
		// Cookie line
		else if (line_items.size() > 1 && line_items[0] == "Cookie:")
		{
			_parseCookieString(line_items);
			_parseCookieVariables(_header.cookie_string);
		}
		++line_index;
	}
}


// GET /YoupiBanane/blabla.prout

void	Request::findLocation(Server & serv)
{
	std::string URL_to_check = _header.resource_path;
	std::string	prefix;
	
	location_parse_loop:

	// find the right location
	for (size_t i = 0; i < serv.getRoutes().size(); ++i)
	{
		prefix = serv.getRoutes()[i].getPrefix();
		
		// check if the location matches
		if (URL_to_check == prefix)
		{
			_targetLocation = &serv.getRoutes()[i];
			return ;
		}
		else if (prefix[prefix.size() - 1] == '*')
		{
			prefix.erase(prefix.end() - 1);
			if (prefix.compare(0, prefix.size(), URL_to_check))
			{
				_targetLocation = &serv.getRoutes()[i];
				return ;
			}
		}
		else if (prefix[0] == '*' && URL_to_check.size() >= prefix.size())
		{
			prefix.erase(prefix.begin());
			if (URL_to_check.compare(URL_to_check.size() - prefix.size(), prefix.size(), prefix))
			{
				_targetLocation = &serv.getRoutes()[i];
				return ; 
			}
		}
	}

	URL_to_check.erase(URL_to_check.end() - 1);
	if (URL_to_check.empty())
	{
		// if no location checks
		_validRequest = false;
		_error = NOT_FOUND;
	}
	else
	{
		while(URL_to_check.size() >= 1 && URL_to_check[URL_to_check.size() - 1] != '/')
			URL_to_check.erase(URL_to_check.end() - 1);
		// this condition covers the case of an URL without '/' at the beginning (bad syntax)
		if (URL_to_check.empty())
		{
			_validRequest = false;
			_error = BAD_REQUEST;
		}
		goto location_parse_loop;
	}
}

int		Request::checkAccess()
{
	std::string absolute_path = _targetLocation->getRoot() + 
		(_header.resource_path[0] == '/' ? "" : "/") + _header.resource_path;

	int 		check = 0;
	struct stat sb;

<<<<<<< HEAD
	if (_body.type == "directory" && _header.method == "POST")
		return 0;
=======
	// for GET and DELETE and file-overwriting POST

>>>>>>> 678a33529403c03415278a13fa54c55e4dff5185
	if (!access(absolute_path.c_str(), F_OK) || (
		stat(absolute_path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)
	))
	{
		if (_header.method == "GET")
			check = access(absolute_path.c_str(), R_OK);
		else if (_header.method == "POST")
			check = access(absolute_path.c_str(), W_OK);
		else if (_header.method == "DELETE")
			check = access(absolute_path.c_str(), W_OK);
		else
			check = !0;
		
		if (check != 0)
		{
			setError(FORBIDDEN);
			setIsRequestValid(false);
			return 1;
		}
	}
	else
	{
		if (_header.method == "POST") // we need to check if location folder is W & R & X
			return 0;
		setError(NOT_FOUND);
		setIsRequestValid(false);
		return 1;
	}
	return 0;
}


int		Request::identifyType()
{
	std::string file_to_check = _header.resource_path;
	std::string extension;

	std::string::reverse_iterator rit = file_to_check.rbegin();
	for (; rit != file_to_check.rend(); ++rit)
	{
		if (*rit == '.')
			break ;
		if (*rit == '/')
		{
			_body.type = "directory";
			_body.isMedia = false;
			if (!_targetLocation->isAutoindexed() && _header.method == "GET")
			{
				file_to_check = _targetLocation->getIndexPage();
				extension.clear();
				rit = file_to_check.rbegin();
				rit--;
				_header.resource_path.append(file_to_check);
				continue ;
			}
			return 0;
		}
		extension.insert(extension.begin(), *rit);
	}
	
	if (extension.empty())
	{
		setError(BAD_REQUEST);
		setIsRequestValid(false);
		return 1;
	}
	
	// DELETE requests can take any kind of file (or directory?)
	if (_header.method == "DELETE")
	{
		_body.type = extension;
		return 0;
	}

	if (extension == "png" || extension == "ico" || extension == "jpg" || extension == "gif")
	{
		_body.isMedia = true;
		_body.type = "image/" + extension;
	}
	else if (extension == "html")
	{
		_body.isMedia = false;
		_body.type = "text/html";
	}
	else if (extension == "css")
	{
		_body.isMedia = false;
		_body.type = "text/css";
	}
	else if (extension == "js")
	{
		_body.isMedia = false;
		_body.type = "text/javascript";
	}
	else if (extension == "mpeg" || extension == "mp3")
	{
		_body.isMedia = true;
		_body.type = "audio/" + extension;
	}
	else if (extension == "mp4")
	{
		_body.isMedia = true;
		_body.type = "audio/mp4";
	}
	else if (extension == "py" || extension == "php" || extension == "pl")
	{
		_body.type = extension;
	}
	else
	{
		setError(BAD_REQUEST);
		setIsRequestValid(false);
		return 1;
	}
	
	return 0;
}

Server	*Request::enableVirtualServer(GlobalConfiguration *globalConf, const Server & server)
{
	Server *serv = NULL;
	
	if ((server.getIP() == "localhost" || server.getIP() == "127.0.0.1") &&
		(_header.host.substr(0, 9) == "localhost" || _header.host.substr(0, 9) == "127.0.0.1"))
	{
		return serv;
	}

	for (size_t i = 0; i < globalConf->getServersList().size(); ++i)
	{
		serv = &(globalConf->getServersList()[i]);
		
		for (size_t n = 0; n < serv->getNames().size(); ++n)
		{

			if (_header.host == serv->getNames()[n])
			{
				return serv;
			}
		}
	}

	// if no virtual server is found
	setIsRequestValid(false);
	setError(NOT_FOUND);
	
	return NULL;	
}

void			Request::_parseURL()
{
	std::vector<std::string> path_and_query = split(_header.URL, '?');
	
	if (path_and_query.size() == 2)
	{
		_header.resource_path = path_and_query[0];
		// uniformize resource_path format if it's a folder ('/' at the end)
		if (_header.resource_path.find('.') == std::string::npos &&
			str_back(_header.resource_path) != '/')
			_header.resource_path.append("/"); 
		
		_header.query_string = path_and_query[1];
		// parsing query string into the map
	}
	else if (path_and_query.size() == 1)
	{
		_header.resource_path = path_and_query[0];
		// uniformize resource_path format if it's a folder ('/' at the end)
		if (_header.resource_path.find('.') == std::string::npos &&
			str_back(_header.resource_path) != '/')
			_header.resource_path.append("/"); 
	}
	else
	{
		_validRequest = false;
		_error = BAD_REQUEST;
	}
}

void			Request::_parseCookieString(std::vector<std::string> line_items)
{
	for (size_t i = 1; i < line_items.size(); i++)
	{
		_header.cookie_string += line_items[i];
	}
	
}

void			Request::_parseCookieVariables(std::string cookie_string)
{
	std::vector<std::string> cookies = split(cookie_string, ";");
}

int		Request::getPostData(std::string requestData)
{
	if (_header.method != "POST")
		return 0;

	std::string::reverse_iterator rit = requestData.rbegin();
	while (*rit != '\n')
		rit++;
	while (rit != requestData.rbegin())
	{
		_body.content.push_back(*rit);
		rit--;
	}
	if (rit == requestData.rbegin())
		_body.content.push_back(*rit);
	_body.length = _body.content.size();

	return 0;	
}

void			Request::setError(int err)
{
	_error = err;
}

void			Request::setIsRequestValid(bool val)
{
	_validRequest = val;
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

RequestBody &	Request::getBody()
{
	return _body;
}

RequestHeader &	Request::getHeader()
{
	return _header;
}

bool			Request::isValid() const
{
	return _validRequest;
}

int				Request::getError() const
{
	return _error;	
}

const Location *Request::getTargetLocation()
{
	return _targetLocation;
}

/* ************************************************************************** */