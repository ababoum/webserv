/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 19:54:34 by mababou           #+#    #+#             */
/*   Updated: 2022/08/04 16:28:20 by mababou          ###   ########.fr       */
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
		
		std::string inputPath(av[1]);
		GlobalConfiguration globalConf;
		ConfigurationParser parser(inputPath, globalConf);	

		ServerEngine	engine1(globalConf.getServersList()[0]);
		ServerEngine	engine2(globalConf.getServersList()[1]);

		nfds_t nfds = globalConf.getServersList().size() * 2;
		std::vector<struct pollfd *> fds_ptr;

		fds_ptr.push_back(engine1.getInFdPtr());
		fds_ptr.push_back(engine1.getOutFdPtr());
		fds_ptr.push_back(engine2.getInFdPtr());
		fds_ptr.push_back(engine2.getOutFdPtr());


		int ready;
		
		while (42)
		{
			std::vector<struct pollfd> fds;
			for (std::vector<struct pollfd *>::iterator it = fds_ptr.begin(); it != fds_ptr.end() ; ++it)
				fds.push_back(**it);

			// std::cout << "Waiting for a new connection...\n";

			ready = poll(fds.data(), nfds, 1000 * 30); // timeout = 30s
			if (ready == 0)
			{
				std::cerr << RED_TXT
				<< "webserv timed out: cannot stay idle for more than 30s" 
				<< RESET_TXT << '\n';
				return (EXIT_SUCCESS);
			}
			else if (ready == -1)
			{
				std::cerr << RED_TXT << "poll failed to execute" << RESET_TXT << '\n';
				return (EXIT_SUCCESS);
			}
		
			if (fds[0].revents & POLLIN)
			{
				// std::cout << "stream_in on server 0\n";	
				engine1.stream_in();
			}
			else if (fds[1].revents & POLLOUT)
			{
				// std::cout << "stream_out on server 0\n";
				engine1.stream_out();
			}
			else if (fds[2].revents & POLLIN)
			{
				// std::cout << "stream_in on server 1\n";
				engine2.stream_in();
			}
			else if (fds[3].revents & POLLOUT)
			{
				// std::cout << "stream_out on server 1\n";	
				engine2.stream_out();
			}
		}
	}

	catch (const std::exception & e)	{
		std::cerr << e.what() << '\n';
	}
	
	return 0;
}