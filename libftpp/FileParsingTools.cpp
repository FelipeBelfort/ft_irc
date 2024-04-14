/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileParsingTools.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TheTerror <jfaye@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 20:14:28 by TheTerror         #+#    #+#             */
/*   Updated: 2024/02/24 20:30:24 by TheTerror        ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Libftpp.hpp"

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
