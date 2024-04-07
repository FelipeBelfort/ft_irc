/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TheTerror <jfaye@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 20:43:46 by TheTerror         #+#    #+#             */
/*   Updated: 2024/04/07 20:35:32 by TheTerror        ###   ########lyon.fr   */
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

#define	LISTEN_BACKLOG 20
#define	BUFF_SIZE 500

int main(int argc, char *argv[])
{
	int					fdbk;
	int					sfd, cfd;
	struct addrinfo		hints;
	struct addrinfo		*result, *rp;
	struct sockaddr_in	peer_addr;
	socklen_t			peer_addrlen, tmp_peer_addrlen;
	char				msg[BUFF_SIZE] = "";

	if (argc != 2)
		return (Libftpp::error((std::string) argv[0] + " <port>"));
	sfd = -111;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;
	result = NULL;
	rp = NULL;
	fdbk = getaddrinfo(NULL, argv[1], &hints, &result);
	if (fdbk)
		return (Libftpp::error((std::string) "getaddrinfo(): " + gai_strerror(fdbk)));
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd < 0)
		return (Libftpp::error(""), perror("socket()"), 1);
	for (rp = result; rp; rp = rp->ai_next)
	{
		if (bind(sfd, rp->ai_addr, rp->ai_addrlen) < 0)
			continue;
		else
			break;
	}
	freeaddrinfo(result);
	if (!rp)
		return (Libftpp::error("could not bind"));
	cfd = -111;
	memset(&peer_addr, 0, sizeof(peer_addr));
	peer_addrlen = sizeof(peer_addr);
	tmp_peer_addrlen = peer_addrlen;
	while (1)
	{
		// std::cout << "listen!" << std::endl;
		if (listen(sfd, LISTEN_BACKLOG) < 0)
			return (Libftpp::error(""), perror("listen()"), 1);
		// std::cout << "accept a pending connection!" << std::endl;
		cfd = accept(sfd, (sockaddr*) &peer_addr, &peer_addrlen);
		if (cfd < 0)
			return (Libftpp::error(""), perror("accept()"), 1);
		if (peer_addrlen > tmp_peer_addrlen)
			return (Libftpp::error("accept(): warning!!! address was truncated"), 1);
		std::cout << "connection established!" << std::endl;
		if (read(cfd, &msg, BUFF_SIZE) < 0)
			return (Libftpp::error(""), perror("read()"), 1);
		std::cout << "message: '" << msg << "'" << std::endl;
		break;
	}
	close(sfd);
	return (0);
}

