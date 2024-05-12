/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 20:26:43 by TheTerror         #+#    #+#             */
/*   Updated: 2024/05/10 19:28:04 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User(int sockfd) : _sockfd(sockfd), _registered(false)
{
	this->_map["CAP"] = &User::capCommand;
	this->_map["PASS"] = &User::passCommand;
	this->_map["NICK"] = &User::nickCommand;
	this->_map["USER"] = &User::userCommand;
	this->_map["QUIT"] = &User::quitCommand;
	// this->_map["LUSERS"] = &User::lusersCommand;
	this->_map["PING"] = &User::pingCommand;
	this->_map["JOIN"] = &User::joinCommand;
	this->_map["PRIVMSG"] = &User::privmsgCommand;
	this->_map["KICK"] = &User::kickCommand;
	this->_map["INVITE"] = &User::inviteCommand;
	this->_map["MODE"] = &User::modeCommand;
	this->_map["TOPIC"] = &User::topicCommand;
}

User::~User(void)
{
	onUserRemove();
}
/**
 * @brief it always firstly concatenates the old buffers 
 * 		contents stuck in 'inmsg' with the new buffer content.
 * 		Then launch the parsing and then send eventual messages
 * 		wrote in 'outmsg' to the remote.
 * 
 * @param buff : buffer filled on the last reading step
 * @return true, false, _ignore, _fatal, _error
 */
int		User::routine(const size_t& index, std::string buff)
{
	int		fdbk;

	fdbk = true;
	this->_inmsg += buff;
	if (this->_inmsg.find_first_of('\n') != this->_inmsg.npos)
	{
/*DEBUG*/std::cout << "inmsg: '" + this->_inmsg << "'" << std::endl;
		fdbk = parse(index);
		if (!fdbk)
			return (fdbk);
		//parsing
	}
	// else
	// 	return (true); //TODO review the if/else
	
	if (!Server::broadcastMsg.empty())
		Server::broadcasting();
	if (!this->_outmsg.empty() && (Server::getSockrevents(index) & POLLOUT))
	{
		if (!transmitMessage(index))
			return (false);
	}
// /*DEBUG*/std::cerr << "*** debugging *** fdbk = " + Libftpp::itoa(fdbk) << '\n';
	return (fdbk);
}

/**
 * @brief transmits the messages inserted in 'outmsg' 
 * 		by users and channels to this client.
 * 		After transmission, successfully transmitted 
 * 		characters are erased from 'outmsg' such that 
 * 		if transmission was partial, the pending content 
 * 		might be transmitted on next call.
 * @param index index of this user's socket in sockets container
 * @return int 
 */
int	User::transmitMessage(const size_t& index)
{
	int		send_ret;

	send_ret = -1;
	send_ret = send(Server::getSockfd(index), this->_outmsg.c_str(), \
		this->_outmsg.size(), MSG_DONTWAIT);
	if (send_ret < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
		return (Libftpp::error("send() would block"), _ignore);
	if (send_ret < 0)
		return (Libftpp::ft_perror("send()"));
/*DEBUG*/std::cout << "outmsg: '" + this->_outmsg << "'" << std::endl;
	if ((size_t) send_ret < this->_outmsg.size())
	{
		this->_outmsg.erase(0, send_ret);
		return (true);
	}
	this->_outmsg.clear();
	return (true);
}

int		User::motd_rpl_numerics(void)
{
	std::string		filename;

	filename = Server::getMsgOfTheDay();
	if (filename.empty())
		this->_outmsg += Server::numericMessage(HOSTNAME, ERR_NOMOTD, \
			this->_nickname, "MOTD File is missing");
	else
	{
		std::string		line;
		std::string		motd;
		std::ifstream	is;

		try
		{
			is.exceptions(std::ios_base::badbit | std::ios_base::failbit);
			is.open(filename.c_str(), std::ios_base::in);
			is.exceptions(std::ios_base::goodbit);
		}
		catch(const std::exception& e)
		{
			std::cerr << "Error: On openning of MOTD's file:" << e.what() << '\n';
			return (false);
		}
		while (is)
		{
			line.clear();
			std::getline(is, line);
			if (!line.empty())
				motd += line;
		}
		this->_outmsg += Server::numericMessage(HOSTNAME, ERR_NOMOTD, \
			this->_nickname, motd);
	}
	return (true);
}

/**
 * @brief parses the line received from the network and 
 * 		launches the eventual command's handling
 * 
 * @param index 
 * @return int 
 */
int	User::parse(const size_t& index)
{
	int					fdbk;
	std::string			line;
	std::istringstream	istr;

	fdbk = true;
// /*DEBUG*/ std::cout << "inmsg: '" + this->_inmsg + "'" << std::endl;
	Libftpp::trimStart(this->_inmsg, " \t\r\n");
	while (this->_inmsg.find_first_of('\n', 0) != this->_inmsg.npos)
	{
		this->_command.clear();
		this->_parameters.clear();
		line = Libftpp::extractStr(this->_inmsg, "\n", true);
		istr.str(line);
		while (istr && this->_command.empty())
		{
			this->_command.clear();
			std::getline(istr, this->_command, ' ');
			if (this->_command.empty())
				Libftpp::error("empty command field");
		}
		while (istr && this->_parameters.empty())
		{
			this->_parameters.clear();
			std::getline(istr, this->_parameters, '\n');
			if (this->_parameters.empty())
				Libftpp::error("empty parameters field");
		}
		if (!this->_command.empty() && !this->_parameters.empty())
		{
			try
			{
				fdbk = (this->*_map.at(this->_command))(index);
				if (fdbk == _fatal)
					return (fdbk);
					// return (false);
			}
			catch(const std::exception& e)
			{
/*DEBUG*/		std::cerr << "Unknown COMMAND '" << this->_command << "' :: parse() ->" << e.what() << '\n';
				// return (true);
			}
// std::cerr << "command: " << this->_command << "\n";
// std::cerr << "parameters: " << this->_parameters << "\n";
		}
	}
	return (fdbk);
}
