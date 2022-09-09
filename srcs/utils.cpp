/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 20:13:24 by mababou           #+#    #+#             */
/*   Updated: 2022/09/09 17:21:04 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"

// CPP spliter
std::vector<std::string>	split(const std::string &str, char delim)
{
	std::size_t i = 0;
	std::vector<std::string> list;

	std::size_t pos = str.find(delim);

	while (pos != std::string::npos)
	{
		list.push_back(str.substr(i, pos - i));
		i = ++pos;
		pos = str.find(delim, pos);
	}

	list.push_back(str.substr(i, str.length()));

	return list;
}

std::vector<std::string>	split(const std::string &str, const char *set_delim)
{
	std::size_t i = 0;
	std::vector<std::string> list;

	std::size_t pos = str.find_first_of(set_delim);

	while (pos != std::string::npos)
	{
		if (pos == i)
		{
			i = ++pos;
		}
		else
		{
			list.push_back(str.substr(i, pos - i));
			i = ++pos;
		}
		pos = str.find_first_of(set_delim, pos);
	}
	
	if (i != str.length())
		list.push_back(str.substr(i, str.length()));

	return list;
}

std::string 				media_to_string(const char *img_path)
{
	std::ifstream 		media_file;
	std::vector<char> 	tab;
	char				byte;

	media_file.open(img_path);
	if (!media_file)
	{
		// string is empty if image file cannot be opened
		return std::string("<title>Media Page Not Found</title><H1>Media Page Not Found</H1>");
	}
	while (media_file.get(byte)) {
        tab.push_back(byte);
    }
	
	std::string ret(tab.begin(), tab.end());
	media_file.close();
	return ret;	
}

std::string					htmlPath_to_string(const char *html_path)
{
	std::string		ret;
	std::ifstream	file;

	file.open(html_path, std::ios::in);
	if (!file)
	{
		ret += "<title>Error Page Not Found</title><H1>Error Page Not Found</H1>";
		return ret;
	}
	for (std::string line; std::getline(file, line);)
	{
		ret.append(line);
	}
	file.close();

	return ret;
}

std::string					int_to_string(int n)
{
	return SSTR("" << n);
}

bool						is_digit(std::string str)
{
	for (size_t i = 0; i < str.size(); ++i)
	{
		if (!std::isdigit(str[i]))
			return false;
	}
	return true;
}

bool 	operator==(struct pollfd rhs, struct pollfd lhs)
{
	return rhs.fd == lhs.fd;
}

bool 	operator<(struct pollfd rhs, struct pollfd lhs)
{
	return rhs.fd < lhs.fd;
}
