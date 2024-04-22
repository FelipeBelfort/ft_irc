/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileParsingTools.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 20:14:28 by TheTerror         #+#    #+#             */
/*   Updated: 2024/04/21 19:41:26 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Libftpp.hpp"

/**
 * @brief authenticate a filename by parsing the mandatory part
 * 		and eventually the extension part relative to extension parameter
 * @param filename the name of the file to be parse
 * @param extension the eventual accurate extension
 * @param errorfield holds the eventual error message in case of error
 * 
 * @return true
 * @return false
 */
bool	Libftpp::authFileExtension(const std::string& filename, \
				const std::string& extension, std::string& errorfield)
{
	int					i;
	std::string			substr;
	std::istringstream	stream(filename);

	i = 0;
	errorfield.clear();
	if (filename.empty())
		return (errorfield.assign("empty file name"), false);
	if (extension.empty() || Libftpp::strIsWSpaces(extension))
		return (errorfield.assign("given extension is empty"), false);
	while (stream)
	{
		std::getline(stream, substr, '.');
		i++;
	}
	if (i == 2)
		return (errorfield.assign("no extension"), false);
	else if (substr == extension)
		return (true);
	else
		return (errorfield.assign("invalid extension \"." + substr + "\""), false);
	return (false);
}
