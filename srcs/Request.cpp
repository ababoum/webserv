/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 12:11:55 by mababou           #+#    #+#             */
/*   Updated: 2022/09/27 20:35:56 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Request.hpp"


/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Request::Request():
	_header(),
	_body(),
	_validRequest(true),
	_error(NO_ERROR),
	_targetLocation(NULL),
	_raw_data()	{}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Request::~Request() {}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/



/*
** --------------------------------- METHODS ----------------------------------
*/

/**
 * Parses the header of the current request stored in the server engine.
 * Populates attributes (method, content type, cookies, etc.)
 * 
 */
void	Request::parseData()
{
	std::string req_string(_raw_data.begin(), _raw_data.end());

	// extract header only
	size_t 		header_sep = req_string.find("\r\n\r\n");
	if (header_sep != std::string::npos)
	{
		req_string.resize(header_sep);
	}


	// for information only
	std::cerr << "Request received:\n"
			<< BLUE_TXT << req_string << RESET_TXT << '\n' << std::endl;

	std::istringstream 	data(req_string);
	std::string			line;
	size_t				line_index = 1;

	if (_raw_data.empty())
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
			_header.host = line_items[1].substr(0, line_items[1].rfind(':'));
		}
		// User-Agent line
		else if (line_items.size() > 1 && line_items[0] == "User-Agent:")
		{
			_header.user_agent = line.substr(12, line.length() - 13);
		}
		// Cookie line
		else if (line_items.size() > 1 && line_items[0] == "Cookie:")
		{
			_header.cookie_string = line.substr(7);
			if (_header.cookie_string[_header.cookie_string.length() - 1] == '\r')
				_header.cookie_string.erase(_header.cookie_string.end() - 1);
		}
		// Content-Type line
		else if (line_items.size() > 1 && line_items[0] == "Content-Type:")
		{
			if (line_items[1][(line_items[1]).size() - 1] == ';')
				_header.content_type = (line_items[1]).substr(0, (line_items[1]).size() - 1);
			else
				_header.content_type = (line_items[1]).substr(0, (line_items[1]).size());
			if (line_items.size() > 2 && !(line_items[2]).compare(0, 9, "boundary="))
				_header.boundary = "--" + line_items[2].substr(9);
		}
		++line_index;
	}
}


void	Request::findLocation(Server & serv)
{
	std::string URL_to_check = _header.resource_path;
	std::string	req_method = _header.method;
	std::string	prefix;
	
	location_parse_loop:

	// find the right location
	for (size_t i = 0; i < serv.getRoutes().size(); ++i)
	{
		prefix = serv.getRoutes()[i].getPrefix();
		
		// disqualify locations incompatible method-wise
		if (!serv.getRoutes()[i].isAllowedMethod(req_method))
		{
			continue ;
		}
		// check if the location matches
		if (URL_to_check == prefix)
		{
			_targetLocation = &serv.getRoutes()[i];
			return ;
		}
		else if (prefix[0] == '*' && URL_to_check.size() >= prefix.size())
		{
			prefix.erase(prefix.begin());
			if (!URL_to_check.compare(URL_to_check.size() - prefix.size(), prefix.size(), prefix))
			{
				_header.resource_path = getBaseFile(_header.resource_path);
				_targetLocation = &serv.getRoutes()[i];
				return ; 
			}
		}
	}

	// start to shorten the URL to continue location-finding
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
	std::string absolute_path = _targetLocation->getRoot() + "/" + _header.resource_path;
	sanitizePath(absolute_path);
	

	int 		check = 0;
	struct stat sb;

	// for GET and DELETE and file-overwriting POST
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
		if (_header.method == "POST")
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

	if (_targetLocation->isRedirected())
		return 0;
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
	
	// DELETE requests can take any kind of file
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
	else if (extension == "txt")
	{
		_body.isMedia = false;
		_body.type = "text/plain";
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
	else if (_header.method == "POST")
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
	setError(SERVER_ERROR);
	
	return NULL;	
}

void			Request::_parseURL()
{
	std::vector<std::string> path_and_query = split(_header.URL, '?');
	
	if (path_and_query.size() == 2)
	{
		_header.resource_path = path_and_query[0];
		uniformizeFolderPath(_header.resource_path);
		
		_header.query_string = path_and_query[1];
	}
	else if (path_and_query.size() == 1)
	{
		_header.resource_path = path_and_query[0];
		uniformizeFolderPath(_header.resource_path);
	}
	else
	{
		_validRequest = false;
		_error = BAD_REQUEST;
	}
}




int		Request::extractBody()
{
	if (_header.method != "POST")
		return 0;

	
	std::string requestData(_raw_data.begin(), _raw_data.end());
	size_t 		body_sep = requestData.find("\r\n\r\n");

	if (body_sep == std::string::npos)
	{
		_body.content = "";
		_body.length = 0;
		return 0;
	}
	else
	{
		_body.content.insert(_body.content.begin(),
			_raw_data.begin() + body_sep + 4,
			_raw_data.end());
		_body.length = std::distance(_raw_data.begin() + body_sep + 4, _raw_data.end());
	}

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

std::vector<char>	&	Request::getRawData()
{
	return _raw_data;
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