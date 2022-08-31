/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tidurand <tidurand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 12:11:55 by mababou           #+#    #+#             */
/*   Updated: 2022/08/31 10:25:44 by tidurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "../includes/webserv.hpp"

struct RequestHeader
{
	std::string							method;
	std::string							URL;
	std::string							resource_path;
	std::string							query_string;
	std::map<std::string, std::string>	query_variables;
	std::string							post_string;
};

struct RequestBody
{
	std::string		content;
	std::size_t		length;
	std::string		type;
	bool			isMedia;
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
		void	checkAccess();
		void	identifyType();
		void	getPostData(std::string requestData);

		RequestHeader &	getHeader();
		RequestBody &	getBody();
		const Location	*getTargetLocation();
		bool			isValid() const;
		int				getError() const;

		void			setError(int err);
		void			setIsRequestValid(bool val);

	private:
		void			_parseURL();
};


#endif /* ********************************************************* REQUEST_H */