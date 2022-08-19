/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIEngine.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tidurand <tidurand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/13 08:16:38 by tidurand          #+#    #+#             */
/*   Updated: 2022/08/19 09:42:10 by tidurand         ###   ########.fr       */
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

	if (_req->getBody().type == "cgi")
		path = _req->getHeader().resource_path;
	else
		path = _req->getTargetLocation()->getCGI();
	_req = req;
	_body = req->getBody().content;
	
	_env["SERVER_SOFTWARE"] = "";
	_env["SERVER_NAME"] = "";
	_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	_env["SERVER_PORT"] = SSTR( "" << serv->getPort());
	_env["REQUEST_METHOD"] = req->getHeader().method;
	_env["PATH_INFO"] = path;
	_env["PATH_TRANSLATED"] = path;
	_env["SCRIPT_NAME"] = path;
	_env["QUERY_STRING"] = req->getHeader().query_string;
	_env["REMOTE_HOST"] = "";
	_env["REMOTE_ADDR"] = "";
	_env["AUTH_TYPE"] = "";
	_env["REMOTE_USER"] = "";
	_env["REMOTE_IDENT"] = "";
	_env["CONTENT_TYPE"] = req->getBody().type;
	_env["CONTENT_LENGTH"] = SSTR( "" << req->getBody().length);
	_env["HTTP_ACCEPT"] = "";
	_env["HTTP_ACCEPT_LANGUAGE"] = "";
	_env["HTTP_USER_AGENT"] = "";
	_env["HTTP_COOKIE"] = "";
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

	if (_req->getBody().type == "cgi")
	{
		cgi_path.append(_req->getTargetLocation()->getRoot());
		cgi_path.append(_req->getHeader().resource_path);
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
		char * const *n = NULL;
		if (execve(cgi_path.c_str(), n, env) == -1)
		{
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		close(cgi_pipe_read[1]);
		close(cgi_pipe_read[0]);
		close(cgi_pipe_write[1]);
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
		close(cgi_pipe_write[0]);
	}
	free_env(env);
	return ret;
}