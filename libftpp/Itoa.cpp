/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Itoa.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 16:18:31 by TheTerror         #+#    #+#             */
/*   Updated: 2024/04/21 20:27:11 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Libftpp.hpp"

/**
 * @brief Converts an integer value to a null-terminated string
 * 
 * @param n 
 * @return (std::string) string representation of 'n'
 */
std::string		Libftpp::itoa(const size_t& n)
{
	std::string		str;
	size_t			div;
	size_t			mod;

	if (!n)
		return ("0");
	for (mod = n, div = n; div; )
	{
		mod = div % 10;
		div /= 10;
		if (str.empty())
			str.assign(1, mod + 48);
		else
			str.insert(0, 1, mod + 48);
	}
	return (str);
}
