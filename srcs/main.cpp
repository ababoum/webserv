/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 19:54:34 by mababou           #+#    #+#             */
/*   Updated: 2022/08/01 20:32:49 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"
# include "../includes/ConfigurationParser.hpp"
# include "../includes/GlobalConfiguration.hpp"
# include "../includes/Location.hpp"
# include "../includes/Server.hpp"

int main(int ac, char **av)
{
	(void)ac;
	std::string inputPath(av[1]);
	
	GlobalConfiguration globalConf;
	ConfigurationParser(inputPath, globalConf);
	
	return 0;
}