/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 19:54:34 by mababou           #+#    #+#             */
/*   Updated: 2022/08/15 17:51:41 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

int main(int ac, char **av)
{
	try {
		if (ac < 2)
		{
			std::cerr << RED_TXT << "Error: webserv needs a configuration file to be launched\n"
				<< RESET_TXT << "usage: ./webserv [file.conf]" << std::endl;
			
			return (EXIT_FAILURE);
		}

		if (ac > 2)
		{
			std::cerr << RED_TXT << "Warning: any argument after the first one will be ignored\n"
				<< RESET_TXT << "usage: ./webserv [file.conf]" << std::endl;
		}

		signal(SIGINT, ft_exit);
		
		std::string inputPath(av[1]);
		GlobalConfiguration globalConf;
		ConfigurationParser parser(inputPath, globalConf);	
		globalConf.startEngines();

		nfds_t nfds = globalConf.getFdsPtr().size();
		std::vector<struct pollfd *> *fds_ptr = &globalConf.getFdsPtr();

		int ready;
		
		while (42)
		{
			std::vector<struct pollfd> fds;
			for (std::vector<struct pollfd *>::iterator it = fds_ptr->begin(); it != fds_ptr->end() ; ++it)
				fds.push_back(**it);


			ready = poll(fds.data(), nfds, -1); // timeout = never
			if (ready == 0)
			{
				std::cerr << RED_TXT
				<< "webserv timed out" << RESET_TXT << '\n';
				return (EXIT_SUCCESS);
			}
			else if (ready == -1)
			{
				std::cerr << RED_TXT << "poll failed to execute" << RESET_TXT << '\n';
				return (EXIT_SUCCESS);
			}

			globalConf.dispatchStream(fds);
		}
	}

	catch (const std::exception & e)	{
		std::cerr << RED_TXT << e.what() << '\n' << RESET_TXT;
	}
	
	return (EXIT_SUCCESS);
}