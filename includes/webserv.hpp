/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 20:07:54 by mababou           #+#    #+#             */
/*   Updated: 2022/08/02 18:41:04 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h> // For sockaddr_in
# include <arpa/inet.h> // For inet_addr
# include <unistd.h>
# include <stdio.h>
# include <cstdlib>
# include <string>
# include <iostream>

# include <vector>
# include <map>
# include <set>
# include <fstream>
# include <string>
# include <iostream>
# include <sstream> 

# include "ConfigurationParser.hpp"
# include "GlobalConfiguration.hpp"
# include "Location.hpp"
# include "Server.hpp"
# include "ServerEngine.hpp"

# define RED_TXT	"\e[31m"
# define GREEN_TXT	"\e[32m"
# define YELLOW_TXT	"\e[33m"
# define BLUE_TXT	"\e[34m"
# define MAG_TXT	"\e[35m"

# define BOLD_TXT	"\e[1m"
# define UL_TXT		"\e[4m"

# define RESET_TXT	"\e[0m"


# define DEFAULT_MAX_CONNECTIONS	1024

// Libft Equivalent
void ft_memset(void *ptr, int data, size_t len);


// CPP functions
std::vector<std::string> split(const std::string &str, char delim);
std::vector<std::string> split(const std::string &str, const char *set_delim);


#endif