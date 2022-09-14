/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 12:11:55 by mababou           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2022/09/07 11:30:03 by tidurand         ###   ########.fr       */
=======
/*   Updated: 2022/09/09 15:50:16 by mababou          ###   ########.fr       */
>>>>>>> 7014ad756603c970fa903229223397f84f3ca5d6
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
	std::string							host;
	std::string							post_string;
	std::string							cookie_string;
	std::map<std::string, std::string>	cookie_variables;
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
		Server	*enableVirtualServer(GlobalConfiguration *globalConf, const Server & server);
		void	findLocation(Server & serv);
		int		checkAccess();
		int		identifyType();
		int		getPostData(std::string requestData);

		RequestHeader &	getHeader();
		RequestBody &	getBody();
		const Location	*getTargetLocation();
		bool			isValid() const;
		int				getError() const;

		void			setError(int err);
		void			setIsRequestValid(bool val);

	private:
		void			_parseURL();
		void			_parseCookieString(std::vector<std::string> line_items);
		void			_parseCookieVariables(std::string cookie_string);
};


#endif /* ********************************************************* REQUEST_H */