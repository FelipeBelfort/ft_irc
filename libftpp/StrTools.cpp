/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StrTools.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TheTerror <jfaye@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 20:12:50 by TheTerror         #+#    #+#             */
/*   Updated: 2024/02/25 19:25:52 by TheTerror        ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Libftpp.hpp"

std::string&		Libftpp::trimStart(std::string& str, const std::string& set)
{
	while (!str.empty() && set.find(str.at(0)) != std::string::npos)
	{
		str.erase(0, 1);
	}
	return (str);
}

std::string&		Libftpp::trimStart(std::string& str, const std::string& set, const size_t& nb)
{
	size_t	n;

	n = nb;
	while (!str.empty() && n && set.find(str.at(0)) != std::string::npos)
	{
		str.erase(0, 1);
		n--;
	}
	return (str);
}

std::string&		Libftpp::trimEnd(std::string& str, const std::string& set)
{
	while (!str.empty() && set.find(str.at(str.size() - 1)) != std::string::npos)
	{
		str.erase(str.size() - 1, 1);
	}
	return (str);
}

std::string&		Libftpp::trimEnd(std::string& str, const std::string& set, const size_t& nb)
{
	size_t	n;

	n = nb;
	while (!str.empty() && n && set.find(str.at(str.size() - 1)) != std::string::npos)
	{
		str.erase(str.size() - 1, 1);
		n--;
	}
	return (str);
}

std::string&		Libftpp::trim(std::string& str, const std::string& set)
{
	str = Libftpp::trimStart(str, set);
	str = Libftpp::trimEnd(str, set);
	return (str);
}

std::string&		Libftpp::trim(std::string& str, const std::string& set, const size_t& nb)
{
	str = Libftpp::trimStart(str, set, nb);
	str = Libftpp::trimEnd(str, set, nb);
	return (str);
}

std::string&		Libftpp::removeChars(std::string& str, const std::string& set)
{
	for (size_t i = 0; !str.empty() && i < str.size(); )
	{
		if (set.find(str.at(i)) != std::string::npos)
			str.erase(i, 1);
		else
			i++;
	}
	return (str);
}

bool				Libftpp::strIsWSpaces(const std::string& str)
{
	for (std::string::const_iterator it = str.begin(); \
			it != str.end(); it++)
	{
		if (!std::isspace(*it))
			return (false);
	}
	return (true);
}

bool				Libftpp::strIsDouble(std::string str)
{
	size_t	i;

	i = 0;
	str = strToLower(str);
	if (str.empty())
		return (false);
	if (str == "-inf" || str == "nan" || str == "+inf" || str == "inf")
		return (true);
	if (str == ".")
		return (false);
	if (str.at(i) == '-' || str.at(i) == '+')								// (optional) plus or minus sign 
		i++;
	if (i < str.size() && str.at(i) == '.' && i + 1 < str.size() && !std::isdigit(str.at(i + 1)))
		return (false);
	for (i = i; i < str.size() && std::isdigit(str.at(i)); i++)				// nonempty sequence of decimal digits
	{}
	if (i >= str.size() || str.at(i) != '.' )								// decimal-point character 
		return (false);
	for (i += 1; i < str.size() && std::isdigit(str.at(i)); i++)			// nonempty sequence of decimal digits
	{}
	if (i < str.size() && str.at(i) == 'e')									// (optional) 'e' or 'E' defining exponent to base 10)
	{
		i++;
		if (i < str.size() && (str.at(i) == '+' || str.at(i) == '-'))		// (optional) plus or minus exponent sign
			i++;
		if (i < str.size() && std::isdigit(str.at(i)))
		{
			for (i += 1; i < str.size() && std::isdigit(str.at(i)); i++)	// nonempty sequence of decimal digits
			{}
		}
		else
			return (false);
	}
	if (i >= str.size())
		return (true);
	return (false);
}


std::string			Libftpp::strToLower(std::string str)
{
	std::string		lowstr;

	lowstr = str;
	if (str.empty())
		return (str);
	for (size_t i = 0; i < str.size(); i++)
		lowstr[i] = std::tolower(str.at(i));
	return (lowstr);
}

bool				Libftpp::strIsInt(std::string str)
{
	size_t	i;

	i = 0;
	if (str.empty())
		return (false);
	if (str.at(i) == '-' || str.at(i) == '+')					// (optional) plus or minus sign 
		i++;
	for (i = i; i < str.size(); i++)							// nonempty sequence of decimal digits
	{
		if (!std::isdigit(str.at(i)))
			return (false);
	}
	return (true);
}

bool				Libftpp::strIsFloat(std::string str)
{
	size_t	i;

	i = 0;
	str = strToLower(str);
	if (str.empty())
		return (false);
	if (str == "-inff" || str == "nanf" || str == "+inff" || str == "inff")
		return (true);
	if (str == "f" || str == "+f" || str == "-f")
		return (true);
	if (str == ".f" || !str.compare(0, 2, ".e"))
		return (false);
	if (str.at(i) == '-' || str.at(i) == '+')						// (optional) plus or minus sign 
		i++;
	if (i >= str.size() || (!std::isdigit(str.at(i)) && str.at(i) != '.'))
		return (false);
	for (i = i; i < str.size() && std::isdigit(str.at(i)); i++)		// nonempty sequence of decimal digits
	{}
	if (i >= str.size() || str.at(i) != '.')						// decimal-point character 
		return (false);
	for (i += 1; i < str.size() && std::isdigit(str.at(i)); i++)	// nonempty sequence of decimal digits
	{}
	if (i < str.size() && str.at(i) == 'e')									// (optional) 'e' or 'E' defining exponent to base 10)
	{
		i++;
		if (i < str.size() && (str.at(i) == '+' || str.at(i) == '-'))		// (optional) plus or minus exponent sign
			i++;
		if (i < str.size() && std::isdigit(str.at(i)))
		{
			for (i += 1; i < str.size() && std::isdigit(str.at(i)); i++)	// nonempty sequence of decimal digits
			{}
		}
		else
			return (false);
	}
	if (i < str.size() && str.at(i) == 'f' && i + 1 >= str.size())			// character 'f' or 'F' specifying float-type
		return (true);
	return (false);
}
