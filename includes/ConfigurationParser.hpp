/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationParser.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 15:08:25 by mababou           #+#    #+#             */
/*   Updated: 2022/08/21 20:35:54 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATIONPARSER_HPP
# define CONFIGURATIONPARSER_HPP

# include "../includes/webserv.hpp"

class GlobalConfiguration;
class Server;
class Location;

class ConfigurationParser
{
	public:
		class parsing_error: public std::exception
		{
			private:
				const char * _msg;
			
			public:
				parsing_error() {}
				parsing_error(const char * msg): _msg(msg) {}
				virtual const char * what () const throw ()
				{
					return _msg;
				}
		};

		class syntax_error: public std::exception
		{
			private:
				const char * _msg;
			public:
				syntax_error() {}
				syntax_error(const char * msg): _msg(msg) {}
				virtual const char * what () const throw ()
				{
					return _msg;
				}
		};

	public:

		// parser cannot be constructed without conf file and global conf
		ConfigurationParser(std::string inputFilePath, GlobalConfiguration & globalConf);
		ConfigurationParser( ConfigurationParser const & src );
		~ConfigurationParser();

		ConfigurationParser &	operator=( ConfigurationParser const & rhs );

	private:
	
		void	_parseFile();
		void	_parseLine(std::vector<std::string> & line_items, std::size_t line_nb);
		void	_parseServerLine(std::vector<std::string> & line_items, std::size_t line_nb);
		void	_parseLocationLine(std::vector<std::string> & line_items, std::size_t line_nb);
		void	_checkServerDuplicate();
		void	_checkCurrentServerIntegrity(std::size_t line_nb) const;
		void	_checkCurrentLocationIntegrity(std::size_t line_nb) const;
		void	_solveCurrentLocationIntegrity();

	private:
		std::string				_inputFilePath;
		GlobalConfiguration &	_globalConf;

	private:

		// used for the parsing
		std::string				_context;
		Server*					_currentServer;
		Location*				_currentLocation;

};

#endif /* ********************************************* CONFIGURATIONPARSER_H */