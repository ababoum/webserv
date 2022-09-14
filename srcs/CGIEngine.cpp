/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIEngine.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/13 08:16:38 by tidurand          #+#    #+#             */
/*   Updated: 2022/09/14 18:59:37 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/CGIEngine.hpp"


static void	free_env(char **env)
{
	for (size_t i = 0; env[i]; i++)
		delete env[i];
	delete [] env;
}

CGIEngine::CGIEngine(Request *req, Server *serv)
{	
	std::string path;
	std::string content;

	_req = req;
	_body = req->getBody().content;
	std::cout << YELLOW_TXT<< "body : " << _body << RESET_TXT<<std::endl;
	if (_req->getBody().type == "cgi")
	{
		path = _req->getTargetLocation()->getRoot();
		path += _req->getHeader().resource_path;
		_content = path;
		// _content = htmlPath_to_string(path.c_str());
		// std::cerr << path << std::endl;
	}
	else
	{
		content = _req->getTargetLocation()->getRoot();
		content += "/";
		content += _req->getTargetLocation()->getIndexPage();
		_content = content;
		// _content = htmlPath_to_string(content.c_str());
		path = _req->getTargetLocation()->getCGI();
	}
	
	_env["SERVER_SOFTWARE"] = "Webserv/1.0";
	_env["SERVER_NAME"] = serv->getIP();
	_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	_env["SERVER_PORT"] = int_to_string(serv->getPort());
	_env["REQUEST_METHOD"] = req->getHeader().method;
	_env["PATH_INFO"] = path;
	_env["PATH_TRANSLATED"] = path;
	_env["SCRIPT_NAME"] = path;
	_env["SCRIPT_FILENAME"] = "www/index2.php";
	_env["QUERY_STRING"] = req->getHeader().query_string;
	_env["REMOTE_HOST"] = "";
	_env["REMOTE_ADDR"] = "";
	_env["AUTH_TYPE"] = "";
	_env["REMOTE_USER"] = "";
	_env["REMOTE_IDENT"] = "";
	_env["CONTENT_TYPE"] = "application/x-www-form-urlencoded";
	_env["CONTENT_LENGTH"] = int_to_string(req->getBody().length);
	_env["REDIRECT_STATUS"] = "200"; // careful to what status code to fill
	_env["HTTP_ACCEPT"] = "*/*";
	_env["HTTP_ACCEPT_LANGUAGE"] = "en-US,en";
	_env["HTTP_USER_AGENT"] = "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/105.0.0.0 Safari/537.36";
	_env["HTTP_COOKIE"] = "fname=a";
	_env["HTTP_REFERER"] = "http://localhost:9900/test_method.php?fname=a&lname=a";
}

CGIEngine::~CGIEngine()
{
}

char	**CGIEngine::mapToStr(std::map<std::string, std::string> env)
{
	char **c_env = new char* [env.size() + 1];
	size_t j = 0;
	for (std::map<std::string, std::string>::iterator i = env.begin(); i != env.end(); i++)
	{
		std::string key_value = i->first + "=" + i->second;
		c_env[j] = new char[key_value.size() + 1];
		strcpy(c_env[j], key_value.c_str());
		j++;
	}
	return c_env;
}


std::string		CGIEngine::exec()
{
	pid_t 		pid;
	char 		**env;
	int 		cgi_pipe_read[2];
	int 		cgi_pipe_write[2];
	std::string ret;
	int			status;
	char		buffer[CGI_BUFFER_SIZE + 1] = {0};
	std::string	cgi_path;

	if (_req->getBody().type == "cgi")
	{
		cgi_path.append("www/cgi/php-cgi");
	}
	else
	{
		cgi_path.append(_req->getTargetLocation()->getRoot());
		cgi_path += (_req->getTargetLocation()->getCGI()[0] == '/' ? "" : "/");
		cgi_path.append(_req->getTargetLocation()->getCGI());
	}
	env = mapToStr(_env);
	pipe(cgi_pipe_read);
	pipe(cgi_pipe_write);
	
	pid = fork();
	if (pid == -1)
	{
		std::cerr << RED_TXT << "Fork failed\n" << RESET_TXT;
		free_env(env);
		throw std::runtime_error("accept");
	}
	if (pid == 0)
	{
		// input request body into 'input fd' of CGI script
		write(cgi_pipe_read[1], _body.c_str(), _body.size());	
		close(cgi_pipe_read[1]);
		dup2(cgi_pipe_read[0], STDIN_FILENO);
		close(cgi_pipe_read[0]);

		// catch output of the CGI script into output pipe
		dup2(cgi_pipe_write[1], STDOUT_FILENO);
		close(cgi_pipe_write[1]);
		char *arg[3];
		arg[0] = const_cast<char*>(cgi_path.c_str());
		arg[1] = const_cast<char*>(_content.c_str());
		arg[2] = NULL;

		std::cerr << RED_TXT <<"type: " << _env["CONTENT_TYPE"] << '\n';
		std::cerr << RED_TXT  <<"length: " << _env["CONTENT_LENGTH"] << '\n';
		std::cerr << RED_TXT  <<"filename: " << _env["SCRIPT_FILENAME"] << '\n';
		std::cerr << RED_TXT  <<"method: " << _env["REQUEST_METHOD"] << '\n';
		std::cerr << RED_TXT  <<"body: " << _body << '\n';

		std::cerr << RESET_TXT ;
		
		if (execve(arg[0], arg, env) == -1)
		{
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		close(cgi_pipe_read[1]);
		close(cgi_pipe_read[0]);
		close(cgi_pipe_write[1]);
		waitpid(pid, &status, 0);
		// check if execve failed and set appropriate error page
		if (WEXITSTATUS(status) == EXIT_FAILURE)
		{
			_req->setError(SERVER_ERROR);
			_req->setIsRequestValid(false);

			return ret;
		}
		
		while (read(cgi_pipe_write[0], buffer, CGI_BUFFER_SIZE) > 0)
		{
			ret.append(buffer);
		}
		// std::cerr << "RET : " << ret << std::endl;
		close(cgi_pipe_write[0]);
	}
	free_env(env);
	std::cout << ret << std::endl;
	return ret;
}