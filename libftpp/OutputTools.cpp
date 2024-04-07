/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OutputTools.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TheTerror <jfaye@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 13:32:09 by TheTerror         #+#    #+#             */
/*   Updated: 2024/02/25 13:32:33 by TheTerror        ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Libftpp.hpp"

std::string			Libftpp::putFloat(float f)
{
	std::string			void_string;
	long long			int_part;
	float				dec_part;

	void_string.clear();
	int_part = static_cast<long long>(f);
	dec_part = f - int_part;
	if (dec_part == 0)
		std::cout << int_part << ".0f";
	else
		std::cout << f << "f";
	return (void_string);
}

std::string			Libftpp::putDouble(double d)
{
	std::string			void_string;
	long long			int_part;
	double				dec_part;

	void_string.clear();
	int_part = static_cast<long long>(d);
	dec_part = d - int_part;
	if (dec_part == 0)
		std::cout << int_part << ".0";
	else
		std::cout << d;
	return (void_string);
}
