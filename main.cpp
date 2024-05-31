/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfaye <jfaye@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 20:26:46 by TheTerror         #+#    #+#             */
/*   Updated: 2024/05/30 18:20:32 by jfaye            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "./server/Server.hpp"

int main(int argc, char **argv)
{
	signal(SIGINT, &exitServer);
	if (argc != 3)
	{
		std::cerr << "Error: Expected ./ircserv <port> <password>" << std::endl;
		return (1);
	}
	try
	{
		if (!Server::launchServer(argv[1], argv[2]))
			return (1);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}