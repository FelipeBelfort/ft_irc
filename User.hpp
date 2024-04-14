/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 20:26:29 by TheTerror         #+#    #+#             */
/*   Updated: 2024/04/14 13:51:53 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include <poll.h>
# include <sys/socket.h>
# include <iostream>
# include <string>
# include <sstream>
# include <map>
# include "Server.hpp"
# include "libftpp/Libftpp.hpp"

class User
{
	private:
		pollfd*		_user_socket;
		std::string	_password;
		std::string	_nickname;
		std::string	_username;
		std::string	_hostname;
		std::string	_servername;
		std::string	_realname;
		std::string	_command;
		std::string	_parameters;
		// int			_fd;
		// bool		_is_logged;
		std::string	_inmsg;
		std::string	_outmsg;
		std::map<std::string, bool (User:: *)(void)>	_map;

	public:

		User(pollfd& user_socket);
		~User(void);

		bool	routine(std::string buff);
		bool	parse();
		bool	authenticate();
		bool	nickCommand();
		bool	userCommand();
		// bool	quit();
		// bool	join();
		// bool	invite();
		// bool	topic();
		// bool	modes();
		// bool	kick();
		void	getMessage(std::string& msg);
		// void	joinChannel(std::string channel);
		std::string	sendMessage(); // std::string msg, std::string channel);

		const std::string	getNick(void);
};

#endif