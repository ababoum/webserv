/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 12:40:05 by mababou           #+#    #+#             */
/*   Updated: 2022/08/13 20:25:08 by mababou          ###   ########.fr       */
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

	ResponseHeader();
	static const std::string default_protocol;
	
	std::string		getText() const;
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

		void			setStatusCode(int code);
		void			setStatusMsg(std::string msg);
		void			setContentType(std::string type);
		void			setContentLength(std::size_t length);
		void			setBody(std::string body);
		void			setFromCGI(bool val);
		// for CGI response only
		void			setCGIText(std::string CGI_text);
		void			reset();
};


#endif /* ******************************************************** RESPONSE_H */