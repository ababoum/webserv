/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 20:07:54 by mababou           #+#    #+#             */
/*   Updated: 2022/09/26 11:36:38 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h> // For sockaddr_in
# include <arpa/inet.h>	// For inet_addr
# include <sys/stat.h> // For directory testing
# include <unistd.h>
# include <stdio.h>
# include <poll.h> // for poll
# include <string.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <dirent.h> // For directory listing

# include <cstdlib>
# include <vector>
# include <map>
# include <set>
# include <fstream>
# include <string>
# include <iostream>
# include <sstream>

class ServerEngine;
struct Connection;
typedef	std::map<struct pollfd, ServerEngine *> 	t_fds_map;

# include "ConfigurationParser.hpp"
# include "GlobalConfiguration.hpp"
# include "Location.hpp"
# include "Server.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "ServerEngine.hpp"
# include "CGIEngine.hpp"

bool 	operator==(struct pollfd rhs, struct pollfd lhs);
bool	operator<(struct pollfd rhs, struct pollfd lhs);

# define RED_TXT "\e[31m"
# define GREEN_TXT "\e[32m"
# define YELLOW_TXT "\e[33m"
# define BLUE_TXT "\e[34m"
# define MAG_TXT "\e[35m"

# define BOLD_TXT "\e[1m"
# define ITALIC_TXT "\e[2m"
# define UL_TXT "\e[4m"

# define RESET_TXT "\e[0m"

# define DEBUG(x)		std::cerr << YELLOW_TXT << x << RESET_TXT;
# define FATAL_ERR(x)	std::cerr << RED_TXT << x << RESET_TXT;
# define INFO(x)		std::cerr << ITALIC_TXT << x << RESET_TXT;
# define RUNTIME_MSG(x)	std::cerr << GREEN_TXT << x << RESET_TXT;

# define DEFAULT_MAX_CONNECTIONS 	1024
# define REQUEST_BUFFER_SIZE		1024
# define CGI_BUFFER_SIZE			1024
# define DEFAULT_CLIENT_BODY_SIZE	1000

# define CHUNKED_RESPONSE_SIZE		1000

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
	FOUND = 302, // HTTP redirection
	TEMPORARY_REDIRECT = 307, // HTTP redirection
	PERMANENT_REDIRECT = 308, // HTTP redirection
	BAD_REQUEST = 400,
	FORBIDDEN = 403,
	NOT_FOUND = 404,
	METHOD_NOT_ALLOWED = 405,
	SERVER_ERROR = 500
};

# define PHP_CGI_PATH		"cgi/php-cgi8.1"
# define PYTHON_CGI_PATH	"cgi/python3.10"
# define PERL_CGI_PATH		"cgi/perl"

# define BAD_REQUEST_DEFAULT		"default_error_pages/400.html"
# define FORBIDDEN_DEFAULT			"default_error_pages/403.html"
# define NOT_FOUND_DEFAULT			"default_error_pages/404.html"
# define METHOD_NOT_ALLOWED_DEFAULT	"default_error_pages/405.html"
# define SERVER_ERROR_DEFAULT		"default_error_pages/500.html"

# define SSTR(x) static_cast<std::ostringstream &>(           \
					(std::ostringstream() << std::dec << x)) \
					.str()


// CPP functions
std::vector<std::string>	split(const std::string &str, char delim);
std::vector<std::string>	split(const std::string &str, const char *set_delim);
std::string 				media_to_string(const char *img_path);
std::string					htmlPath_to_string(const char *html_path);
std::string					autoindexPageHtml(std::string directoryName, std::string headerURL);
std::string					int_to_string(int n);
std::string					itohex(std::size_t size);
bool						is_digit(std::string str);
bool						is_folder_formatted(std::string path, bool regexOn = true);
void						sanitizePath(std::string & path);
void						uniformizeFolderPath(std::string & folderPath);
std::string					getParentDirectory(std::string path);
std::string					getBaseFile(std::string path);


#endif