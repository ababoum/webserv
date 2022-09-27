/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 12:40:05 by mababou           #+#    #+#             */
/*   Updated: 2022/09/27 20:05:21 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Response.hpp"


const std::string ResponseHeader::default_protocol = "HTTP/1.1";

/*
** ---------------------------------- HEADER ----------------------------------
*/

ResponseHeader::ResponseHeader()
{
	protocol_version = default_protocol;
}

std::string		ResponseHeader::getText(bool body_is_ifstream) const
{
	std::string ret;

	// line 1
	ret.append(protocol_version);
	ret += ' ';
	ret += std::string(int_to_string(status_code));
	ret += ' ';
	ret.append(status_msg);
	ret += '\n';

	// line 2
	ret.append("Content-Type: ");
	ret.append(content_type);
	ret += '\n';

	if (body_is_ifstream)
		ret.append("Transfer-Encoding: chunked\n");

	return ret;	
}

std::string		ResponseHeader::getRedirText() const
{
	std::string ret;

	// line 1
	ret.append(protocol_version);
	ret += ' ';
	ret += std::string(int_to_string(status_code));
	ret += ' ';
	ret.append(status_msg);
	ret += '\n';

	// line 2
	ret.append("Location: ");
	ret.append(redir_location);
	ret += '\n';

	return ret;
}

std::string		ResponseHeader::getDeleteText() const
{
	std::string ret;

	// line 1
	ret.append(protocol_version);
	ret += ' ';
	ret += std::string(int_to_string(status_code));
	ret += ' ';
	ret.append(status_msg);
	ret += '\n';


	return ret;
}

std::string		ResponseHeader::getCGIText() const
{
	std::string ret;

	// line 1
	ret.append(protocol_version);
	ret += ' ';
	ret += std::string(int_to_string(status_code));
	ret += ' ';
	ret.append(status_msg);
	ret += "\r\n";

	// line 2
	ret.append("Content-Type: ");
	ret.append(content_type);
	ret += "\r\n";

	// line 3
	ret.append("Content-Length: ");
	ret.append(int_to_string(content_length));
	ret += "\r\n";

	// line cookie
	for (std::vector<std::string>::const_iterator it (cookie_line.begin()); it != cookie_line.end(); it++)
	{
		ret.append(*it);
		ret += "\r\n";
	}
	
	//lines session
	if (!expires.empty())
	{
		ret.append(expires);
		ret += "\r\n";
	}
	if (!cache_control.empty())
	{
		ret.append(cache_control);
		ret += "\r\n";
	}
	if (!pragma.empty())
	{
		ret.append(pragma);
		ret += "\r\n";
	}

	return ret;	
}



/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Response::Response() : _from_cgi(false), _validResponse(true)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Response::~Response()
{
	if (_ifstream_body.is_open())
		_ifstream_body.close();
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/



/*
** --------------------------------- METHODS ----------------------------------
*/

void	Response::setValidity(bool val)
{
	_validResponse = val;
}

void	Response::setStatusCode(int code)
{
	_header.status_code = code;
}

void	Response::setStatusMsg(std::string msg)
{
	_header.status_msg = msg;
}

void	Response::setContentType(std::string type)
{
	_header.content_type = type;	
}

void	Response::setContentLength(std::size_t length)
{
	_header.content_length = length;
	_body.length = length;
}

void	Response::setCookieLine(std::vector<std::string> cookie)
{
	_header.cookie_line = cookie;
}

void	Response::setBody(std::string body)
{
	_body.content = body;
}

void	Response::setFromCGI(bool val)
{
	_from_cgi = val;
}

void	Response::setCGIText(std::string CGI_text)
{
	_cgi_output = CGI_text;
}

void	Response::setExpires(std::string expires)
{
	_header.expires = expires;
}

void	Response::setCacheControl(std::string cache)
{
	_header.cache_control = cache;
}

void	Response::setPragma(std::string pragma)
{
	_header.pragma = pragma;
}

void	Response::setRedirectionLocation(std::string url)
{
	_header.redir_location = url;
}

void	Response::setIfstreamBodyHTML(const char *path)
{
	_ifstream_body.open(path);
	if (!_ifstream_body.is_open())
	{
		_body.content = "<title>Error Page Not Found</title><H1>Error Page Not Found</H1>";
		_body.length = 65;
	}
}

void	Response::setIfstreamBodyMedia(const char *path)
{
	_ifstream_body.open(path);
	if (!_ifstream_body.is_open())
	{
		_body.content = "<title>Media Page Not Found</title><H1>Media Page Not Found</H1>";
		_body.length = 65;
	}
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


bool		Response::isValid() const
{
	return _validResponse;
}


std::string	Response::getHeaderText() const
{
	return (_header.getText(_ifstream_body.is_open()));
}

std::string	Response::getRedirText() const
{
	return _header.getRedirText();
}


bool		Response::isFromCGI() const
{
	return _from_cgi;
}

std::string		Response::getCGIText() const
{
	std::string ret_txt;

	ret_txt.append(_header.getCGIText());
	ret_txt += "\r\n";
	
	ret_txt.insert(ret_txt.size(), _body.content);

	return (ret_txt);
}

ResponseHeader	&Response::getHeader()
{
	return _header;
}

ResponseBody	&Response::getBody()
{
	return _body;
}

std::ifstream	&Response::getIfstreamBody()
{
	return _ifstream_body;
}

/* ************************************************************************** */