/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalvie <lcalvie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 20:07:54 by mababou           #+#    #+#             */
/*   Updated: 2022/09/12 02:17:33 by lcalvie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> // For sockaddr_in
#include <arpa/inet.h>	// For inet_addr
#include <sys/stat.h> // For directory testing
#include <unistd.h>
#include <stdio.h>
#include <poll.h> // for poll
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <dirent.h> // For directory listing

#include <cstdlib>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

class ServerEngine;
struct Connection;
typedef	std::map<struct pollfd, ServerEngine *> 	t_fds_map;

#include "ConfigurationParser.hpp"
#include "GlobalConfiguration.hpp"
#include "Location.hpp"
#include "Server.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "ServerEngine.hpp"
#include "CGIEngine.hpp"

bool 	operator==(struct pollfd rhs, struct pollfd lhs);
bool	operator<(struct pollfd rhs, struct pollfd lhs);

#define RED_TXT "\e[31m"
#define GREEN_TXT "\e[32m"
#define YELLOW_TXT "\e[33m"
#define BLUE_TXT "\e[34m"
#define MAG_TXT "\e[35m"

#define BOLD_TXT "\e[1m"
#define UL_TXT "\e[4m"

#define RESET_TXT "\e[0m"

#define DEFAULT_MAX_CONNECTIONS 	1024
#define	REQUEST_BUFFER_SIZE			1024
#define	CGI_BUFFER_SIZE				1024
#define DEFAULT_CLIENT_BODY_SIZE	1000

#define CHUNKED_RESPONSE_SIZE		1000

enum REQUEST_ERROR
{
	NO_ERROR = 0,
	FAULTY_REQUEST = 400
};

enum HTTP_RESPONSE
{
	SUCCESS_OK = 200, // GET, POST
	CREATED = 201, // POST
	MOVED_PERMANENTLY = 301, // HTTP redirection
	BAD_REQUEST = 400,
	FORBIDDEN = 403,
	NOT_FOUND = 404,
	METHOD_NOT_ALLOWED = 405,
	SERVER_ERROR = 500
};

#define SSTR(x) static_cast<std::ostringstream &>(           \
					(std::ostringstream() << std::dec << x)) \
					.str()


// CPP functions
std::vector<std::string>	split(const std::string &str, char delim);
std::vector<std::string>	split(const std::string &str, const char *set_delim);
std::string 				media_to_string(const char *img_path);
std::string					htmlPath_to_string(const char *html_path);
std::string					autoindexPageHtml(std::string directoryName);
std::string					int_to_string(int n);
std::string					itohex(std::size_t size);
bool						is_digit(std::string str);


#endif