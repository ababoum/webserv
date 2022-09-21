/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIEngine.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tidurand <tidurand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/13 08:16:38 by tidurand          #+#    #+#             */
/*   Updated: 2022/09/21 14:41:02 by tidurand         ###   ########.fr       */
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

	_req = req;
	_body = req->getBody().content;
	if (!_body.empty())
		_body.erase(_body.begin());
	if (_req->getHeader().resource_path != "/")
	{
		path = _req->getTargetLocation()->getRoot();
		path += _req->getHeader().resource_path;
		_inputFile = path;
	}
	else
	{
		_inputFile = _req->getTargetLocation()->getRoot();
		_inputFile += "/";
		_inputFile += _req->getTargetLocation()->getIndexPage();
		path = _inputFile;
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
	_env["SCRIPT_FILENAME"] = path;
	_env["QUERY_STRING"] = req->getHeader().query_string;
	_env["REMOTE_HOST"] = req->getHeader().host;
	_env["REMOTE_ADDR"] = serv->getIP();
	_env["AUTH_TYPE"] = "";
	_env["REMOTE_USER"] = "";
	_env["REMOTE_IDENT"] = "";
	_env["CONTENT_TYPE"] = "application/x-www-form-urlencoded";
	_env["CONTENT_LENGTH"] = int_to_string(req->getBody().length);
	_env["REDIRECT_STATUS"] = "200"; // careful to what status code to fill
	_env["HTTP_ACCEPT"] = "*/*";
	_env["HTTP_ACCEPT_LANGUAGE"] = "en-US,en";
	_env["HTTP_USER_AGENT"] = req->getHeader().user_agent;
	_env["HTTP_COOKIE"] = req->getHeader().cookie_string;
	_env["HTTP_REFERER"] = "";
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
	char 		*arg[4] = {0};
	// std::string php_flag("-q");

	if (_req->getBody().type == "php") 
	{
		cgi_path = PHP_CGI_PATH;
		// arg[1] = const_cast<char*>(php_flag.c_str());
		arg[1] = const_cast<char*>(_inputFile.c_str());
	}
	else if (_req->getBody().type == "py") 
	{
		cgi_path = PYTHON_CGI_PATH;
		arg[1] = const_cast<char*>(_inputFile.c_str());
	}
	else if (_req->getBody().type == "pl") 
	{
		cgi_path = PERL_CGI_PATH;
		arg[1] = const_cast<char*>(_inputFile.c_str());
	}
	arg[0] = const_cast<char*>(cgi_path.c_str());
	env = mapToStr(_env);
	pipe(cgi_pipe_read);
	pipe(cgi_pipe_write);

	
	pid = fork();
	if (pid == -1)
	{
		std::cerr << RED_TXT << "Fork failed\n" << RESET_TXT;
		free_env(env);
		throw std::runtime_error("fork");
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

		// std::cerr << arg[0] << '\n';
		// std::cerr << arg[1] << '\n';
		// std::cerr << arg[2] << '\n';
		// std::cerr << (arg[3] == 0) << '\n';

		if (execve(arg[0], arg, env) == -1)
		{
			exit(EXIT_FAILURE);
		}
		// system("cgi/php-cgi7.4 -q www/test_get.php");
		exit (EXIT_SUCCESS);
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
			close(cgi_pipe_write[0]);

			return ret;
		}
		
		int tot = 0;
		int r = 0;


		pipe_read:
		r = read(cgi_pipe_write[0], buffer, CGI_BUFFER_SIZE);
		if (r > 0)
		{
			ret += (buffer);
			tot += r;
			memset(buffer, 0, CGI_BUFFER_SIZE);
			goto pipe_read;
		}
		close(cgi_pipe_write[0]);
		
	}
	free_env(env);
	return ret;
}