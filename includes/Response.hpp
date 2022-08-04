/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 12:40:05 by mababou           #+#    #+#             */
/*   Updated: 2022/08/04 12:42:16 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "../includes/webserv.hpp"

struct ResponseBody
{
	std::string		content;
	std::size_t		length;
}

struct ResponseHeader
{
	std::string		protocolVersion;
	int				status_code;
	std::string		status_msg;
	std::string		server_name;
}


class Response
{

	public:

		Response();
		~Response();

	private:

};


#endif /* ******************************************************** RESPONSE_H */