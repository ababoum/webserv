/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 19:54:34 by mababou           #+#    #+#             */
/*   Updated: 2022/09/22 17:46:22 by mababou          ###   ########.fr       */
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
		signal(SIGINT, ft_exit);
		
		if (ac < 2)
		{
			FATAL_ERR("Error: webserv needs a configuration file to be launched\n");
			INFO("usage: ./webserv [file.conf]\n");
			
			return (EXIT_FAILURE);
		}

		if (ac > 2)
		{
			FATAL_ERR("Warning: any argument after the first one will be ignored\n");
			INFO("usage: ./webserv [file.conf]\n");
		}

		// check if all cgi binaries are available
		if (access(PHP_CGI_PATH, X_OK) || 
			access(PYTHON_CGI_PATH, X_OK) || 
			access(PERL_CGI_PATH, X_OK))
		{
			FATAL_ERR("Error: webserv cannot work properly without executable CGI binaries in cgi/\n");
			INFO("solution: provide php-cgi, python, and perl in cgi/ with execution privileges\n");

			return (EXIT_FAILURE);
		}

		// check if all default error pages exist
		if (access(BAD_REQUEST_DEFAULT, R_OK) || 
			access(FORBIDDEN_DEFAULT, R_OK) || 
			access(NOT_FOUND_DEFAULT, R_OK) ||
			access(METHOD_NOT_ALLOWED_DEFAULT, R_OK) ||
			access(SERVER_ERROR_DEFAULT, R_OK))
		{
			FATAL_ERR("Error: webserv cannot work properly without default error pages\n");
			INFO("solution: provide default error pages in default_error_pages/\n");

			return (EXIT_FAILURE);
		}
		
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
				FATAL_ERR("webserv timed out\n");
				return (EXIT_SUCCESS);
			}
			else if (ready == -1)
			{
				FATAL_ERR("poll failed to execute\n");
				return (EXIT_SUCCESS);
			}

			DEBUG("ready FDs: " << ready << '\n');
			DEBUG("FD0: " << fds.data()[0].revents);
			DEBUG("FD1: " << fds.data()[1].revents);

			globalConf.dispatchStream(fds);
		}
	}

	catch (const std::exception & e)	{
		FATAL_ERR(e.what() << '\n');
		return (EXIT_FAILURE);
	}
	catch (int sig_code) {
		std::cout << GREEN_TXT << "\nQuitting webserv... Thanks!\n" << RESET_TXT;
		return (sig_code);
	}
	
	return (EXIT_SUCCESS);
}