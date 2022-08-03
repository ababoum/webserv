/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 19:54:34 by mababou           #+#    #+#             */
/*   Updated: 2022/08/03 13:42:27 by mababou          ###   ########.fr       */
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
		ServerEngine	engine2(globalConf.getServersList()[1]);

		nfds_t nfds = globalConf.getServersList().size();
		struct pollfd *fds = (struct pollfd *)malloc(nfds * sizeof(struct pollfd));
		if (fds == NULL)
			return (EXIT_FAILURE);

		fds[0].fd = engine1.getSocketFd();
		fds[0].events = POLLIN | POLLOUT;
		fds[1].fd = engine2.getSocketFd();
		fds[1].events = POLLIN | POLLOUT;

		int ready;

		
		while (42)
		{
			std::cout << "Waiting for a new connection...\n";

			ready = poll(fds, nfds, 1000 * 10); // timeout = 10s
			if (ready == 0)
			{
				std::cerr << "webserv timed out: cannot stay idle for more than 10s" << '\n';
				return (EXIT_SUCCESS);
			}
			if (fds[0].revents & POLLIN )
				engine1.run();
			else if (fds[1].revents & POLLIN)
				engine2.run();
		}

		// engine1.run();
		// engine2.run();
	}

	catch (const std::exception & e)	{
		std::cerr << e.what() << '\n';
	}
	
	return 0;
}