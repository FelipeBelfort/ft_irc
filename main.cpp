/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TheTerror <jfaye@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 20:26:46 by TheTerror         #+#    #+#             */
/*   Updated: 2024/04/11 20:26:47 by TheTerror        ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int main(int argc, char **argv)
{
	// Server	server;

	if (argc != 3)
	{
		std::cout << "Error: Expected ./ircserv <port> <password>" << std::endl;
		return 1;
	}
	// signal(SIGINT, &exitServer);
	try
	{
		// server.launchServer(argv[1], argv[2]);
		Server::launchServer(argv[1], argv[2]);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}

	return 0;
}