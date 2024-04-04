/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TheTerror <jfaye@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 20:43:46 by TheTerror         #+#    #+#             */
/*   Updated: 2024/04/04 21:17:16 by TheTerror        ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <netdb.h>


int main(void)
{
	struct protoent*	p = NULL;
	p = getprotobyname("ip");
	if (p)
	{
		std::cout << "name: " << p->p_name \
		<< "\nprotocol number: " << p->p_proto << std::endl;
		std::cout << "other names:" << std::endl;
		for (size_t i = 0; p->p_aliases[i]; i++)
		{
			std::cout << "* " << p->p_aliases[i] << std::endl;
		}
	}
	std::cout << "------------------------------------------------" << std::endl;
	p = getprotobyname("ipv6");
	if (p)
	{
		std::cout << "name: " << p->p_name \
		<< "\nprotocol number: " << p->p_proto << std::endl;
		std::cout << "other names:" << std::endl;
		for (size_t i = 0; p->p_aliases[i]; i++)
		{
			std::cout << "* " << p->p_aliases[i] << std::endl;
		}
	}
	std::cout << "------------------------------------------------" << std::endl;
	p = getprotobyname("tcp");
	if (p)
	{
		std::cout << "name: " << p->p_name \
		<< "\nprotocol number: " << p->p_proto << std::endl;
		std::cout << "other names:" << std::endl;
		for (size_t i = 0; p->p_aliases[i]; i++)
		{
			std::cout << "* " << p->p_aliases[i] << std::endl;
		}
	}
	return 0;
}