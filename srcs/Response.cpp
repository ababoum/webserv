/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tidurand <tidurand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 12:40:05 by mababou           #+#    #+#             */
/*   Updated: 2022/08/15 09:05:03 by tidurand         ###   ########.fr       */
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

std::string		ResponseHeader::getText() const
{
	std::string ret;

	// line 1
	ret.append(protocol_version);
	ret += ' ';
	ret += std::string(SSTR("" << status_code));
	ret += ' ';
	ret.append(status_msg);
	ret += '\n';

	// line 2
	ret.append("Content-Type: ");
	ret.append(content_type);
	ret += '\n';

	// line 3
	ret.append("Content-Length: ");
	ret.append(SSTR("" << content_length));
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


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string	Response::getText() const
{
	std::string ret_txt;

	ret_txt.append(_header.getText());
	ret_txt += '\n';
	
	ret_txt.append(_body.content);

	return (ret_txt);
}

std::size_t	Response::size() const
{
	if (_from_cgi == false)
		return _header.getText().size() + _body.content.size() + 1;
	else
		return _cgi_output.size();
}

bool		Response::isFromCGI() const
{
	return _from_cgi;
}

std::string		Response::getCGIText() const
{
	return _cgi_output;
}


/* ************************************************************************** */