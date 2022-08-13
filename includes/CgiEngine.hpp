/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiEngine.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tidurand <tidurand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/13 08:15:19 by tidurand          #+#    #+#             */
/*   Updated: 2022/08/13 10:28:50 by tidurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIENGINE_HPP
# define CGIENGINE_HPP

# include "../includes/webserv.hpp"

class Request;

class CgiEngine
{
	public:
		CgiEngine(Request req);
		~CgiEngine();
		
		void	exec();
		char	**mapToStr(std::map<std::string, std::string> env);
		
	private:
		std::map<std::string, std::string> _env;
		std::string _body;
};


#endif