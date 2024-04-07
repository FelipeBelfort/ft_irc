/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Itoa.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TheTerror <jfaye@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 16:18:31 by TheTerror         #+#    #+#             */
/*   Updated: 2024/02/23 17:57:13 by TheTerror        ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Libftpp.hpp"

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
