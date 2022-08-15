/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tidurand <tidurand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 19:13:53 by mababou           #+#    #+#             */
/*   Updated: 2022/08/15 14:10:19 by tidurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "../includes/webserv.hpp"

class Location
{

	public:

		Location();
		Location( Location const & src );
		~Location();

		Location &		operator=( Location const & rhs );

	private:
		
		std::string					_prefix;
		std::string					_root;
		std::set<std::string>		_allowedMethods;
		std::string					_indexPage;
		bool						_autoindex;
		std::string					_cgi;

	public:

		// Setters
		
		void			setPrefix(std::string prefix);
		void			setRoot(std::string path);
		void			addAllowedMethod(std::string method);
		void			setIndexPage(std::string indexPagePath);
		void			setAutoindex(std::string on_off);
		void			setCGI(std::string cgiPath);

		// Accessors

		std::string		getPrefix() const;
		std::string		getIndexPage() const;
		bool			isAutoindexed() const;
		std::string		getCGI() const;
		std::string		getRoot() const;
	
		// Methods
		
		bool			isAllowedMethod(std::string method) const;
		

};


#endif /* ******************************************************** LOCATION_H */