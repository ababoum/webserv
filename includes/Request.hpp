/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 12:11:55 by mababou           #+#    #+#             */
/*   Updated: 2022/08/13 20:42:26 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "../includes/webserv.hpp"

struct RequestHeader
{
	std::string		method;
	std::string		URL;
};

struct RequestBody
{
	std::string		content;
	std::size_t		length;
};

class Request
{

	public:
		Request();
		~Request();

	private:
		RequestHeader	_header;
		RequestBody		_body;
		bool			_validRequest;
		int				_error;
		const Location	*_targetLocation;

	public:

		void	parseData(std::string requestData);
		void	findLocation(Server & serv);

		RequestHeader &	getHeader();
		RequestBody &	getBody();
		const Location	*getTargetLocation();
		bool			isValid() const;
		int				getError() const;

		void			setError(int err);
		void			setIsRequestValid(bool val);
};


#endif /* ********************************************************* REQUEST_H */