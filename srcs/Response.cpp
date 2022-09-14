/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalvie <lcalvie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 12:40:05 by mababou           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2022/09/14 11:20:37 by tidurand         ###   ########.fr       */
=======
/*   Updated: 2022/09/11 22:17:19 by lcalvie          ###   ########.fr       */
>>>>>>> 7014ad756603c970fa903229223397f84f3ca5d6
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

std::string		ResponseHeader::getText(std::size_t body_size) const
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

	// line 3
	if (body_size > CHUNKED_RESPONSE_SIZE)
		ret.append("Transfer-Encoding: chunked\n");
	else
	{
		ret.append("Content-Length: ");
		ret.append(int_to_string(content_length));
		ret += '\n';
	}

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
	ret += '\n';

	// line 2
	ret.append("Content-Type: ");
	ret.append(content_type);
	ret += '\n';

	// line 3
	ret.append("Content-Length: ");
	ret.append(int_to_string(content_length));
	ret += '\n';
	
	//cookies
	ret.append("Set-Cookie: fname=a");
	ret += '\n';

	return ret;	
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Response::Response()
{
	_from_cgi = false;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Response::~Response()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/



/*
** --------------------------------- METHODS ----------------------------------
*/

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

void	Response::reset()
{
	_header.status_code = 0;
	_header.status_msg = "";
	_header.content_length = 0;
	_header.content_type = "";
	
	_body.content = "";
	_body.length = 0;
	
	_file_path = "";
}

void	Response::setRedirectionLocation(std::string url)
{
	_header.redir_location = url;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string	Response::getText() const
{
	std::string ret_txt;

	ret_txt.append(_header.getText(_body.content.size()));
	ret_txt += '\n';
	
	ret_txt.append(_body.content);

	return (ret_txt);
}

std::string	Response::getRedirText() const
{
	return _header.getRedirText();
}

std::size_t	Response::size() const
{
	if (_from_cgi == true)
		return _cgi_output.size();
	else if (!_header.redir_location.empty())
		return _header.getRedirText().size();
	else
		return _header.getText( _body.content.size()).size() + _body.content.size() + 1;
}

bool		Response::isFromCGI() const
{
	return _from_cgi;
}

std::string		Response::getCGIText() const
{
	std::string ret_txt;

	ret_txt.append(_header.getCGIText());
	ret_txt += '\n';
	
	ret_txt.append(_body.content);

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


/* ************************************************************************** */