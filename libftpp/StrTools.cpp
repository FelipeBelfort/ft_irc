/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StrTools.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 20:12:50 by TheTerror         #+#    #+#             */
/*   Updated: 2024/04/26 16:16:28 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Libftpp.hpp"

/**
 * @brief it splits the 'src' param by refering 
 * 		to the set of delim characters, extracts 
 * 		the first substring from 'src' and then returns it.
 * 
 * @param src the given string
 * @param set the set of delim charaters
 * @param preserve_delim premits to assert whether the delim character, if encountered,
 * 		should be kept up on the returned substr
 * @return (std::string) the first substr extracted
 */
std::string		Libftpp::extractStr(std::string& src, \
	const std::string& set, bool preserve_delim)
{
	std::string		extracted;

	while (!src.empty() && set.find(src.at(0)) == set.npos)
	{
		extracted += src.at(0);
		src.erase(0, 1);
	}
	if (!src.empty() && set.find(src.at(0)) != set.npos)
	{
		if (preserve_delim)
			extracted += src.at(0);
		src.erase(0, 1);
	}
	return (extracted);
}

/**
 * @brief removes the given characters in 'set' from the beginning of 'str'
 * 
 * @param str
 * @param set
 * @return std::string& 
 */
std::string&		Libftpp::trimStart(std::string& str, const std::string& set)
{
	while (!str.empty() && set.find(str.at(0)) != std::string::npos)
	{
		str.erase(0, 1);
	}
	return (str);
}

/**
 * @brief removes at most 'nb' characters specified in 'set' 
 * 		from the beginning of 'str'
 * 
 * @param str 
 * @param set 
 * @param nb 
 * @return std::string& 
 */
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

/**
 * @brief removes the given characters in 'set' from the end of 'str'
 * 
 * @param str 
 * @param set 
 * @return std::string& 
 */
std::string&		Libftpp::trimEnd(std::string& str, const std::string& set)
{
	while (!str.empty() && set.find(str.at(str.size() - 1)) != std::string::npos)
	{
		str.erase(str.size() - 1, 1);
	}
	return (str);
}

/**
 * @brief removes at most 'nb' characters specified in 'set' 
 * 		from the end of 'str'
 * 
 * @param str 
 * @param set 
 * @param nb 
 * @return std::string& 
 */
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

/**
 * @brief removes the given characters in 'set' from the beginning 
 * 		and the end of 'str'
 * 
 * @param str 
 * @param set 
 * @return std::string& 
 */
std::string&		Libftpp::trim(std::string& str, const std::string& set)
{
	str = Libftpp::trimStart(str, set);
	str = Libftpp::trimEnd(str, set);
	return (str);
}

/**
 * @brief removes at most 'nb' characters specified in 'set' 
 * 		from the beginning and the end of 'str'
 * 
 * @param str 
 * @param set 
 * @param nb 
 * @return std::string& 
 */
std::string&		Libftpp::trim(std::string& str, const std::string& set, const size_t& nb)
{
	str = Libftpp::trimStart(str, set, nb);
	str = Libftpp::trimEnd(str, set, nb);
	return (str);
}

/**
 * @brief removes the given characters in 'set' from the string 'str' 
 * 
 * @param str 
 * @param set 
 * @return std::string& 
 */
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

/**
 * @brief assert whether the given string holds only whitespaces
 * 
 * @param str 
 * @return true 
 * @return false 
 */
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

/**
 * @brief assert whether the given string is of type double;
 * 		a valid base-10 double number representation
 * 
 * @param str 
 * @return true 
 * @return false 
 */
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

/**
 * @brief turns the given string to a full lower case string
 * 
 * @param str 
 * @return std::string 
 */
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

/**
 * @brief assert whether the given string is of integer type;
 * 		a valid base-10 integer number representation
 * 
 * @param str 
 * @return true 
 * @return false 
 */
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

/**
 * @brief assert whether the given string is of type float;
 * 		a valid base-10 float number representation
 * 
 * @param str 
 * @return true 
 * @return false 
 */
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
