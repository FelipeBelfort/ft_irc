/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 20:26:18 by TheTerror         #+#    #+#             */
/*   Updated: 2024/04/22 20:28:09 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <signal.h>
# include <unistd.h>
# include <fcntl.h>
# include <string>
# include <cstring>
# include <cerrno>
# include <ctime>
# include <sys/socket.h>
# include <netdb.h>
# include <poll.h>
# include <netinet/in.h>
# include <algorithm>
# include <vector>
# include <map>
# include <list>
# include "User.hpp"
# include "./libftpp/Libftpp.hpp"
// # include "Channel.hpp"

# define MIN_PORT 1023
# define MAX_PORT 65535
# define BUFF_SIZE 42
# define MAX_CONNECTIONS 20

class User;

class Server
{
	public:

		Server();
		~Server();

		static bool						is_connected;
		static std::string				sourcename;
		static std::string				_password;
		static std::string				broadcastMsg;


		static bool		launchServer(const std::string &port, const std::string &password);
		static bool 	isUniqueNick(const std::string &nick);
		static bool 	isValidNick(const std::string &nick);
		static bool 	broadcasting(void);
		static int 		getSockfd(const size_t& index);
		static short 	getSockrevents(const size_t& index);
		static const std::string& 	getStartDate();
		static size_t				getNbOfUsers(void);
		static size_t				getMaxOfUsers(void);
		static size_t				getNbOfInvUsers(void);
		static size_t				getNbOfOtherServs(void);
		static size_t				getNbOfOperators(void);
		static size_t				getNbOfUnkConnect(void);
		static size_t				getNbOfChannels(void);
		static const std::string	getMsgOfTheDay(void);
	
	private:
		static uint16_t					_port;
		static std::string				_startDate;
		static std::vector<pollfd>		_sockets;
		static std::map<int, User>		_clients;
		// std::map<std::string, Channel*> _channels;
		static bool	initServer(const std::string &port, const std::string &password);
		static bool	createUser(void);
		static bool	closeClient(int i);
		static bool	loopOnUsers();

};

void	exitServer(int sign);

#endif
