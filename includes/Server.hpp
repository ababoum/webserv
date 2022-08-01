/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 15:14:54 by mababou           #+#    #+#             */
/*   Updated: 2022/08/01 16:23:46 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <set>
# include <vector>
# include <map>

class Server
{

	public:

		Server();
		Server( Server const & src );
		~Server();

		Server &		operator=( Server const & rhs );

	private:

		std::string					_name;
		std::string					_IP;
		int							_port;
		std::set<std::string>		_allowedMethods;
		std::string					_indexPage;
		bool						_autoindex;
		std::map<int, std::string>	_errorPages;
		std::size_t					_clientBufferSize;

	public:

		// Setters
		
		void			setName(std::string name);
		void			setIP(std::string IP); // can be equal to 'localhost'
		void			setPort(std::string port);
		void			addAllowedMethod(std::string method);
		void			setIndexPage(std::string indexPagePath);
		void			setAutoindex(bool on_off);
		void			addErrorPage(int error_code, std::string filePath);
		void			setClientBufferSize(std::size_t buffer_max);

		// Accessors
		
		std::string		getName() const;
		std::string		getIP() const;
		int				getPort() const;
		std::string		getIndexPage() const;
		bool			isAutoindexed() const;
		std::string		getErrorPagePath(int error_code) const;
		std::size_t		getClientBufferSize() const;

		// Methods
		
		bool			isAllowedMethod(std::string method) const;


	private:

		// helper functions
		
		bool _isIPValid(std::string IP) const;
		

};


#endif /* ********************************************************** SERVER_H */