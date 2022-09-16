/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcalvie <lcalvie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 09:39:02 by lcalvie           #+#    #+#             */
/*   Updated: 2022/09/08 19:41:31 by lcalvie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

std::string	autoindexPageHtml(std::string directoryName, std::string headerURL)
{
	std::string body;
	
	if (headerURL.empty() || headerURL[headerURL.length() -1] != '/')
		headerURL+='/';

	body+="<body>\n<h1>Index of "+directoryName+"</h1>\n";
	body+="<pre><a>Name</a></pre><hr />";
	body+="<pre>\n";
	
	DIR *dp = opendir(directoryName.c_str());
	if (dp == NULL)
		return ("<title>Directory Not Found</title><H1>Directory " + directoryName + " Not Found</H1>");
	struct dirent *dirp;
	while ((dirp = readdir(dp)) != NULL) {
		body+= "<a href=\"" + headerURL + dirp->d_name +"\">" + dirp->d_name + "</a>\n";
	}
	closedir(dp);

	body+="</pre>\n</body>\n";

	return (body);
}
