/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 20:26:29 by TheTerror         #+#    #+#             */
/*   Updated: 2024/04/20 16:58:38 by jm               ###   ########lyon.fr   */
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
# include "Include.h"

class User
{
	private:
		pollfd*		_user_socket;
int	_fd;
		std::string	_cap;
		std::string	_password;
		std::string	_nickname;
		std::string	_oldnickname;
		std::string	_username;
		std::string	_hostname;
		std::string	_servername;
		std::string	_realname;
		std::string	_command;
		std::string	_parameters;
		// int			_fd;
		bool		_registered;
		std::string	_inmsg;
		std::string	_outmsg;
		std::map<std::string, int (User:: *)(void)>	_map;

	public:

		User(pollfd& user_socket);
		~User(void);

		std::string	serverMessage(std::string src, std::string cmd, \
			std::string msg);
		std::string	numericMessage(std::string src, std::string num, \
			std::string target, std::string msg);
		std::string	ErrorCommand(std::string msg);
		int		routine(std::string buff);
		int		parse();
		int		capCommand();
		int		authenticate();
		int		nickCommand();
		int		userCommand();
		int		quitCommand();
		int		registrationComplete();
		int		transmitMessage();
		// bool	join();
		// bool	invite();
		// bool	topic();
		// bool	modes();
		// bool	kick();
		void	getMessage(std::string& msg);
		// void	joinChannel(std::string channel);

		const std::string&	getNickname(void) const;
		const std::string&	getOutMsg(void) const;
		bool				isRegistered(void) const;
		void				insertOutMessage(const std::string& msg);
};

#endif