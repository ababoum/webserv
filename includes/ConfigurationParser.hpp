/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationParser.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 15:08:25 by mababou           #+#    #+#             */
/*   Updated: 2022/08/01 21:06:46 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATIONPARSER_HPP
# define CONFIGURATIONPARSER_HPP

# include "../includes/webserv.hpp"

class GlobalConfiguration;

class ConfigurationParser
{
	public:
		class parsing_error: std::exception
		{
			private:
				const char * _err_line;
			
			public:
				parsing_error(const char * error_line): _err_line(error_line) {}
				const char * what () const throw ()
				{
					std::string msg("Parsing error in line ");
					msg.append(_err_line);
					msg.append("\n");

					const char *output = msg.c_str();
					return output;
				}
		};

		class syntax_error: std::exception
		{
			private:
				const char * _msg;
			public:
				syntax_error(const char * msg): _msg(msg) {}
				const char * what () const throw ()
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

	private:
		std::string				_inputFilePath;
		GlobalConfiguration &	_globalConf;

	private:

		// used for the parsing
		std::string				_context;

};

#endif /* ********************************************* CONFIGURATIONPARSER_H */