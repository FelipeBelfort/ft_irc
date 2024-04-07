/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TheTerror <jfaye@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 20:43:46 by TheTerror         #+#    #+#             */
/*   Updated: 2024/04/07 20:35:19 by TheTerror        ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "./libftpp/Libftpp.hpp"


int main(int argc, char *argv[])
{
	int					fdbk;
	int					sfd;
	struct addrinfo		hints;
	struct addrinfo		*result, *rp;

	if (argc != 3)
		return (Libftpp::error((std::string) argv[0] + " <host> <port>"));
	sfd = -111;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;
	hints.ai_flags = 0;
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;
	result = NULL;
	rp = NULL;
	fdbk = getaddrinfo(argv[1], argv[2], &hints, &result);
	if (fdbk)
		return (Libftpp::error((std::string) "getaddrinfo(): " + gai_strerror(fdbk)));
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd < 0)
		return (Libftpp::error(""), perror("socket()"), 1);
	for (rp = result; rp; rp = rp->ai_next)
	{
		if (connect(sfd, rp->ai_addr, rp->ai_addrlen) < 0)
			continue;
		else
			break;
	}
	freeaddrinfo(result);
	if (!rp)
		return (Libftpp::error("could not connect"));
	std::cout << "connected!!" << std::endl;
	write(sfd, "HELLO WORLD!", 13);
	close(sfd);
	return (0);
}