/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 12:11:55 by mababou           #+#    #+#             */
/*   Updated: 2022/08/04 12:27:03 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "../includes/webserv.hpp"

struct RequestBody
{
	std::string		content;
	std::size_t		length;
}

struct RequestHeader
{
	std::string		method;
	std::string		URL;
}

class Request
{

	public:

		Request(std::string requestData);
		~Request();

	private:
		RequestBody		_body;
		RequestHeader	_header;
		bool			_validRequest;
		int				_error;

	public:

		RequestBody &	getBody();
		RequestHeader &	getHeader();
		bool			isValid() const;
		int				getError() const;

};


#endif /* ********************************************************* REQUEST_H */