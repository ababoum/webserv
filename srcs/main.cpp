/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalvie <lcalvie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 19:54:34 by mababou           #+#    #+#             */
/*   Updated: 2022/09/10 01:36:54 by lcalvie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

static void	ft_exit(int sig_code)
{
	throw sig_code;
}

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

		t_fds_map	*fds_ptr = &globalConf.getFdsPtr();

		int ready;
		
		while (42)
		{
			std::vector<struct pollfd> fds;
			for (t_fds_map::iterator it = fds_ptr->begin(); it != fds_ptr->end() ; ++it)
				fds.push_back(it->first);

			nfds_t 		nfds = globalConf.getFdsPtr().size();
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
		return EXIT_FAILURE;
	}
	catch (int sig_code) {
		std::cout << GREEN_TXT << "\nQuitting webserv... Thanks!\n" << RESET_TXT;
		return sig_code;
	}
	
	return (EXIT_SUCCESS);
}