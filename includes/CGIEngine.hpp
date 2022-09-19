/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIEngine.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/13 08:15:19 by tidurand          #+#    #+#             */
/*   Updated: 2022/09/19 14:40:27 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIENGINE_HPP
# define CGIENGINE_HPP

# include "../includes/webserv.hpp"

class Request;

class CGIEngine
{
	public:
		CGIEngine(Request *req, Server *serv);
		~CGIEngine();
		
		std::string		exec();
		char			**mapToStr(std::map<std::string, std::string> env);
		
	private:
		std::map<std::string, std::string>	_env;
		std::string 						_body;
		std::string 						_inputFile;
		Request								*_req;
};


#endif