/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 20:13:24 by mababou           #+#    #+#             */
/*   Updated: 2022/09/23 19:39:29 by mababou          ###   ########.fr       */
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

std::string 				media_to_string(const char *img_path) // a supprimer
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

std::string					htmlPath_to_string(const char *html_path) // a supprimer
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

bool 						operator==(struct pollfd rhs, struct pollfd lhs)
{
	return rhs.fd == lhs.fd;
}

bool 						operator<(struct pollfd rhs, struct pollfd lhs)
{
	return rhs.fd < lhs.fd;
}


std::string					itohex(std::size_t size)
{
	std::string hex;
	do
	{
		hex+= "0123456789ABCDEF"[size%16];
		size /= 16;
	} while (size != 0);

	return std::string(hex.rbegin(), hex.rend());
}


/**
 * Checks if the path is likely to be a folder
 *
 * @param path String containing the path
 * @param regexOn Default value = true. Makes the function take into consideration '*' in the path
 * @return Returns a boolean value. 'true' if the path is likely to be a folder
 */
bool	is_folder_formatted(std::string path, bool regexOn)
{
	for (std::string::reverse_iterator rit = path.rbegin(); rit != path.rend(); ++rit)
	{
		if (*rit == '.')
			return false;
		else if (*rit == '*' && regexOn)
			return true;
		else if (*rit == '/')
			return true;
	}
	return true;
}

/**
 * Transforms a path by replacing all multiple occurences of '/' by a single one.
 * Example: "/folder//folder2/file.out" -> "/folder/folder2/file.out"
 *
 * @param path String containing the original path
 */
void	sanitizePath(std::string & path)
{
	while (path.find("//") != std::string::npos)
	{
		path.erase(path.find("//"), 1);
	}
}

/**
 * Uniformize a folder path by making it end with '/'
 *
 * @param folderPath String containing the original folder path
 */
void	uniformizeFolderPath(std::string & folderPath)
{
	if (is_folder_formatted(folderPath, false))
	{
		folderPath.append("/");
		sanitizePath(folderPath);
	}
}

std::string	getParentDirectory(std::string path)
{
	std::string::reverse_iterator rit = path.rbegin();

	while (!path.empty())
	{
		if (*rit == '/')
			break;
		path.erase(path.end() - 1);
		++rit;
	}
	return path;
}

std::string	getBaseFile(std::string path)
{
	std::string::reverse_iterator rit = path.rbegin();
	std::string base_path;
	
	while (!path.empty())
	{
		if (*rit == '/')
			break;
		base_path.insert(base_path.begin(), *rit);
		++rit;
	}
	return base_path;
}
