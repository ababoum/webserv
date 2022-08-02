/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 19:54:34 by mababou           #+#    #+#             */
/*   Updated: 2022/08/02 18:39:37 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

int main(int ac, char **av)
{
	try {
		if (ac < 2)
		{
			std::cerr << RED_TXT << "Webserv needs a configuration file to be launched\n"
				<< RESET_TXT << "usage: ./webserv [file.conf]" << std::endl;
			
			return (EXIT_FAILURE);
		}

		if (ac > 2)
		{
			std::cerr << RED_TXT << "Warning: any argument after the first one will be ignored\n"
				<< RESET_TXT << "usage: ./webserv [file.conf]" << std::endl;
		}
		
		std::string inputPath(av[1]);
		GlobalConfiguration globalConf;
		ConfigurationParser parser(inputPath, globalConf);	

		ServerEngine	engine1(globalConf.getServersList()[0]);

		engine1.run();
	}

	catch (const std::exception & e)	{
		std::cerr << e.what() << '\n';
	}
	
	return 0;
}