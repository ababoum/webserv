/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 19:13:53 by mababou           #+#    #+#             */
/*   Updated: 2022/08/01 19:54:31 by mababou          ###   ########.fr       */
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

	public:

		// Setters
		
		void			setPrefix(std::string prefix);
		void			setRoot(std::string path);
		void			addAllowedMethod(std::string method);
		void			setIndexPage(std::string indexPagePath);
		void			setAutoindex(bool on_off);

		// Accessors

		std::string		getPrefix() const;
		std::string		getIndexPage() const;
		bool			isAutoindexed() const;
	
		// Methods
		
		bool			isAllowedMethod(std::string method) const;
		

};


#endif /* ******************************************************** LOCATION_H */