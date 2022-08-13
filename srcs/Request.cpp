/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 12:11:55 by mababou           #+#    #+#             */
/*   Updated: 2022/08/13 20:42:20 by mababou          ###   ########.fr       */
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

Request::Request(): _validRequest(true), _error(NO_ERROR)
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

	while(std::getline(data, line, '\n'))
	{
		std::vector<std::string> line_items = split(line, ' ');
		if (line_index == 1)
		{
			if ((line_items[0] != "GET" && line_items[0] != "POST" && line_items[0] != "DELETE") ||
				line_items.size() != 3)
			{
				_error = BAD_REQUEST;
				_validRequest = false;
				return ;
			}
			else
			{
				_header.method = line_items[0];
				_header.URL = line_items[1];	
			}
		}
		++line_index;
	}
}

void	Request::findLocation(Server & serv)
{
	std::string	prefix;
	
	// find the right location
	for (size_t i = 0; i < serv.getRoutes().size(); ++i)
	{
		prefix = serv.getRoutes()[i].getPrefix();
		
		// check if the location matches
		if (_header.URL == prefix)
		{
			_targetLocation = &serv.getRoutes()[i];
			return ;
		}
		else if (prefix[prefix.size() - 1] == '*')
		{
			prefix.erase(prefix.end() - 1);
			if (prefix.compare(0, prefix.size(), _header.URL))
			{
				_targetLocation = &serv.getRoutes()[i];
				return ;
			}
		}
		else if (prefix[0] == '*' && _header.URL.size() >= prefix.size())
		{
			prefix.erase(prefix.begin());
			if (_header.URL.compare(_header.URL.size() - prefix.size(), prefix.size(), prefix))
			{
				_targetLocation = &serv.getRoutes()[i];
				return ; 
			}
		}
	}

	// if no location checks
	_validRequest = false;
	_error = NOT_FOUND;
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