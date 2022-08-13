/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 20:07:54 by mababou           #+#    #+#             */
/*   Updated: 2022/08/13 20:16:49 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> // For sockaddr_in
#include <arpa/inet.h>	// For inet_addr
#include <unistd.h>
#include <stdio.h>
#include <poll.h> // for poll
#include <string.h>
#include <sys/wait.h>

#include <cstdlib>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

#include "ConfigurationParser.hpp"
#include "GlobalConfiguration.hpp"
#include "Location.hpp"
#include "Server.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "ServerEngine.hpp"
#include "CGIEngine.hpp"

#define RED_TXT "\e[31m"
#define GREEN_TXT "\e[32m"
#define YELLOW_TXT "\e[33m"
#define BLUE_TXT "\e[34m"
#define MAG_TXT "\e[35m"

#define BOLD_TXT "\e[1m"
#define UL_TXT "\e[4m"

#define RESET_TXT "\e[0m"

#define DEFAULT_MAX_CONNECTIONS 1024
#define	REQUEST_BUFFER_SIZE		1024
#define	CGI_BUFFER_SIZE			1024

enum REQUEST_ERROR
{
	NO_ERROR = 0,
	FAULTY_REQUEST = 400
};

enum HTTP_RESPONSE
{
	SUCCESS_OK = 200, // GET, POST
	CREATED = 201, // POST
	BAD_REQUEST = 400,
	FORBIDDEN = 403,
	NOT_FOUND = 404,
	SERVER_ERROR = 500
};

#define SSTR(x) static_cast<std::ostringstream &>(           \
					(std::ostringstream() << std::dec << x)) \
					.str()

// Libft Equivalent
void
ft_memset(void *ptr, int data, size_t len);

// CPP functions
std::vector<std::string> split(const std::string &str, char delim);
std::vector<std::string> split(const std::string &str, const char *set_delim);

#endif