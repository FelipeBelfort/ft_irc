/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 20:26:29 by TheTerror         #+#    #+#             */
/*   Updated: 2024/04/22 20:12:28 by jm               ###   ########lyon.fr   */
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
	public:

		User();
		~User(void);

		int		routine(std::string buff, size_t index);

		const std::string&	getNickname(void) const;
		const std::string&	getOutMsg(void) const;
		bool				isRegistered(void) const;
		void				insertOutMessage(const std::string& msg);
		std::string	serverMessage(std::string src, std::string cmd, \
			std::string msg);
		std::string	numericMessage(std::string src, std::string num, \
			std::string target, std::string msg);
		std::string	ErrorCommand(std::string msg);

	private:
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

		int		parse();
		int		capCommand();
		int		authenticate();
		int		nickCommand();
		int		userCommand();
		int		quitCommand();
		int		lusersCommand();
		int		registrationComplete();
		int		transmitMessage(const size_t& index);
		// bool	join();
		// bool	invite();
		// bool	topic();
		// bool	modes();
		// bool	kick();
		// void	joinChannel(std::string channel);
		int		lusers_rpl_numerics(void);
		int		motd_rpl_numerics(void);
};

#endif