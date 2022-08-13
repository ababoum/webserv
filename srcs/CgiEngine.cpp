/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiEngine.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tidurand <tidurand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/13 08:16:38 by tidurand          #+#    #+#             */
/*   Updated: 2022/08/13 10:48:40 by tidurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiEngine.hpp"

CgiEngine::CgiEngine(Request req)
{	
	_env["SERVER_SOFTWARE"] = "";
	_env["SERVER_NAME"] = "";
	_env["GATEWAY_INTERFACE"] = "";
	_env["SERVER_PROTOCOL"] = "";
	_env["SERVER_PORT"] = "";
	_env["REQUEST_METHOD"] = req.getHeader().method;
	_env["PATH_INFO"] = "";
	_env["PATH_TRANSLATED"] = "";
	_env["SCRIPT_NAME"] = "";
	_env["QUERY_STRING"] = "";
	_env["REMOTE_HOST"] = "";
	_env["REMOTE_ADDR"] = "";
	_env["AUTH_TYPE"] = "";
	_env["REMOTE_USER"] = "";
	_env["REMOTE_IDENT"] = "";
	_env["CONTENT_TYPE"] = "";
	_env["CONTENT_LENGTH"] = "";
	_env["HTTP_ACCEPT"] = "";
	_env["HTTP_ACCEPT_LANGUAGE"] = "";
	_env["HTTP_USER_AGENT"] = "";
	_env["HTTP_COOKIE"] = "";
	_env["HTTP_REFERER"] = "";
}

CgiEngine::~CgiEngine()
{
}

char	**CgiEngine::mapToStr(std::map<std::string, std::string> env)
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

void	CgiEngine::exec()
{
	pid_t pid;
	pid = fork();
	if (pid == -1)
		return;
	if (pid == 0)
		execve("../www/test.cgi", NULL, mapToStr(_env));
	else
		return;
}