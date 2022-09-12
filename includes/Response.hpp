/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalvie <lcalvie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 12:40:05 by mababou           #+#    #+#             */
/*   Updated: 2022/09/11 21:57:30 by lcalvie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "../includes/webserv.hpp"

struct ResponseHeader
{
	std::string		protocol_version;
	int				status_code;
	std::string		status_msg;
	std::string		content_type;
	std::size_t		content_length;
	std::string		redir_location; // for HTTP redirection

	ResponseHeader();
	static const std::string default_protocol;
	
	std::string		getText(std::size_t body_size) const;
	std::string		getRedirText() const;
	std::string		getDeleteText() const;

};

struct ResponseBody
{
	std::string		content;
	std::size_t		length;
};

class Response
{

	public:

		Response();
		~Response();

	private:

		ResponseBody	_body;
		ResponseHeader	_header;
		std::string		_file_path;
		bool			_from_cgi;
		std::string		_cgi_output;
	
	public:

		std::string		getText() const;
		std::size_t		size() const;
		bool			isFromCGI() const;
		std::string		getCGIText() const;
		std::string		getRedirText() const;
		ResponseHeader	&getHeader();
		ResponseBody	&getBody();

		void			setStatusCode(int code);
		void			setStatusMsg(std::string msg);
		void			setContentType(std::string type);
		void			setContentLength(std::size_t length);
		void			setBody(std::string body);
		void			setFromCGI(bool val);
		void			setRedirectionLocation(std::string url);
		// for CGI response only
		void			setCGIText(std::string CGI_text);
		void			reset();
};


#endif /* ******************************************************** RESPONSE_H */