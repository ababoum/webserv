/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tidurand <tidurand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 19:13:53 by mababou           #+#    #+#             */
/*   Updated: 2022/09/16 14:16:52 by tidurand         ###   ########.fr       */
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
		
		std::string						_prefix;
		std::string						_root;
		std::set<std::string>			_allowedMethods;
		std::string						_indexPage;
		bool							_autoindex;
		std::pair<int, std::string>		_redirection;
		bool							_isRedirected;

	public:

		// Setters
		
		void			setPrefix(std::string prefix);
		void			setRoot(std::string path);
		void			addAllowedMethod(std::string method);
		void			setIndexPage(std::string indexPagePath);
		void			setAutoindex(std::string on_off);
		void			setRedirection(int code, std::string url);

		// Accessors

		std::string						getPrefix() const;
		std::string						getIndexPage() const;
		bool							isAutoindexed() const;
		std::set<std::string> & 		getAllowedMethods();
		std::string						getRoot() const;
		bool							isRedirected() const;
		std::pair<int, std::string>		getRedirection() const;
		
		// Methods
		
		bool			isAllowedMethod(std::string method) const;
		

};


#endif /* ******************************************************** LOCATION_H */