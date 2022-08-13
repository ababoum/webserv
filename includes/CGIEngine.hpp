/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIEngine.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/13 08:15:19 by tidurand          #+#    #+#             */
/*   Updated: 2022/08/13 19:49:41 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIENGINE_HPP
# define CGIENGINE_HPP

# include "../includes/webserv.hpp"

class Request;

class CGIEngine
{
	public:
		CGIEngine(Request *req);
		~CGIEngine();
		
		std::string		exec();
		char			**mapToStr(std::map<std::string, std::string> env);
		
	private:
		std::map<std::string, std::string>	_env;
		std::string 						_body;
		Request								*_req;
};


#endif