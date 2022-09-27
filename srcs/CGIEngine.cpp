/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIEngine.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/13 08:16:38 by tidurand          #+#    #+#             */
/*   Updated: 2022/09/27 19:26:53 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/CGIEngine.hpp"

static void free_env(char **env)
{
	if (env)
	{
		for (size_t i = 0; env[i]; i++)
			delete [] env[i];
		delete[] env;
	}
}

CGIEngine::CGIEngine(Request *req, Server *serv)
{
	std::string path;

	_req = req;
	_body = req->getBody().content;
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
	if (_req->getHeader().content_type == "multipart/form-data" && !_req->getHeader().boundary.empty())
	{
		_env["CONTENT_TYPE"] = _req->getHeader().content_type + "; boundary=" + _req->getHeader().boundary.substr(2);
		// DEBUG(_env["CONTENT_TYPE"]);
	}
	else if (_req->getHeader().content_type.empty())
		_env["CONTENT_TYPE"] = "application/x-www-form-urlencoded";
	else
		_env["CONTENT_TYPE"] = _req->getHeader().content_type;
	_env["CONTENT_LENGTH"] = int_to_string(req->getBody().length);
	_env["REDIRECT_STATUS"] = "200";
	_env["HTTP_ACCEPT"] = "*/*";
	_env["HTTP_ACCEPT_LANGUAGE"] = "en-US,en";
	_env["HTTP_USER_AGENT"] = req->getHeader().user_agent;
	_env["HTTP_COOKIE"] = req->getHeader().cookie_string;
	_env["HTTP_REFERER"] = "";
}

CGIEngine::~CGIEngine() {}

char **CGIEngine::mapToStr(std::map<std::string, std::string> env)
{
	char **c_env = new char *[env.size() + 1]();
	size_t j = 0;
	
	for (std::map<std::string, std::string>::iterator i = env.begin(); i != env.end(); ++i)
	{
		std::string key_value = i->first + "=" + i->second;
		c_env[j] = new char[key_value.size() + 1]();
		strcpy(c_env[j], key_value.c_str());
		++j;
	}
	return c_env;
}

std::string CGIEngine::exec()
{
	pid_t		pid;
	char 		**env;
	std::string ret;
	int			status;
	char		buffer[CGI_BUFFER_SIZE + 1] = {0};
	std::string	cgi_path;
	char 		*arg[3] = {0};

	if (_req->getBody().type == "php")
	{
		cgi_path = PHP_CGI_PATH;
		arg[1] = const_cast<char *>(_inputFile.c_str());
	}
	else if (_req->getBody().type == "py")
	{
		cgi_path = PYTHON_CGI_PATH;
		arg[1] = const_cast<char *>(_inputFile.c_str());
	}
	else if (_req->getBody().type == "pl")
	{
		cgi_path = PERL_CGI_PATH;
		arg[1] = const_cast<char *>(_inputFile.c_str());
	}
	arg[0] = const_cast<char *>(cgi_path.c_str());
	env = mapToStr(_env);
	char	tmpname_read[] = "/tmp/cgitmpfileXXXXXX";
	char	tmpname_write[] = "/tmp/cgitmpfileXXXXXX";
	int fd_read = mkstemp(tmpname_read);
	if (fd_read == -1)
	{
		free_env(env);
		throw std::runtime_error("mkstemp");
	}
	int fd_write = mkstemp(tmpname_write);
	if (fd_write == -1)
	{
		close(fd_read);
		unlink(tmpname_read);
		free_env(env);
		throw std::runtime_error("mkstemp");
	}
	write(fd_read, _req->getBody().content.c_str(), _req->getBody().content.size());
	lseek(fd_read, 0, SEEK_SET);
	pid = fork();
	if (pid == -1)
	{
		close(fd_read);
		unlink(tmpname_read);
		close(fd_write);
		unlink(tmpname_write);
		FATAL_ERR("Fork failed\n");
		free_env(env);
		throw std::runtime_error("fork");
	}
	if (pid == 0)
	{
		dup2(fd_read, STDIN_FILENO);
		close(fd_read);
		dup2(fd_write, STDOUT_FILENO);
		close(fd_write);
		if (execve(arg[0], arg, env) == -1)
		{
			free_env(env);
			exit(EXIT_FAILURE);
		}
		free_env(env);
		exit(EXIT_SUCCESS);
	}
	else
	{
		waitpid(pid, &status, 0);
		
		if (WEXITSTATUS(status) == EXIT_FAILURE)
		{
			close(fd_read);
			unlink(tmpname_read);
			close(fd_write);
			unlink(tmpname_write);
			free_env(env);
			_req->setError(SERVER_ERROR);
			_req->setIsRequestValid(false);
			return ret;
		}

		lseek(fd_write, 0, SEEK_SET);
		while (read(fd_write, buffer, CGI_BUFFER_SIZE) > 0)
		{
			ret += (buffer);
			memset(buffer, 0, CGI_BUFFER_SIZE);
		}
	}
	close(fd_read);
	unlink(tmpname_read);
	close(fd_write);
	unlink(tmpname_write);
	free_env(env);
	return ret;
}