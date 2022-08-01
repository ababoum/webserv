/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationParser.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mababou <mababou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 15:08:25 by mababou           #+#    #+#             */
/*   Updated: 2022/08/01 15:12:47 by mababou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATIONPARSER_HPP
# define CONFIGURATIONPARSER_HPP

# include <iostream>
# include <string>

class ConfigurationParser
{

	public:

		ConfigurationParser(std::string inputFilePath);
		ConfigurationParser( ConfigurationParser const & src );
		~ConfigurationParser();

		ConfigurationParser &		operator=( ConfigurationParser const & rhs );

	private:
		ConfigurationParser(); // parser cannot be constructed without conf file

		_ 

	private:
		std::string	_inputFilePath;
		

};

#endif /* ********************************************* CONFIGURATIONPARSER_H */