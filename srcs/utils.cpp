/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 20:13:24 by mababou           #+#    #+#             */
/*   Updated: 2022/08/15 19:38:17 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"

// CPP spliter
std::vector<std::string> split(const std::string &str, char delim)
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

std::vector<std::string> split(const std::string &str, const char *set_delim)
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

void	ft_exit(int sig_code)
{
	std::cout << GREEN_TXT << "\nQuiting webserv... Thanks!\n" << RESET_TXT;
	exit(sig_code);
}

std::vector<char> img_to_chars(const char *img_path)
{
	std::ifstream 		img_file;
	std::vector<char> 	ret;
	char				byte;

	img_file.open(img_path);
	if (!img_file)
	{
		// vector is empty if image file cannot be opened
		return ret;
	}
	while (img_file.get(byte)) {
        ret.push_back(byte);
    }
	
	img_file.close();
	return ret;	
}
