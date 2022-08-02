/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 19:54:34 by mababou           #+#    #+#             */
/*   Updated: 2022/08/02 14:20:20 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

int main(int ac, char **av)
{
	try {
		(void)ac;
		std::string inputPath(av[1]);

		GlobalConfiguration globalConf;
		
		ConfigurationParser(inputPath, globalConf);	
	}

	catch (const std::exception & e)	{
		std::cerr << e.what();
	}
	
	return 0;
}