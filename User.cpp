/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 20:26:43 by TheTerror         #+#    #+#             */
/*   Updated: 2024/04/22 20:28:34 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User() : _registered(false)
{
	this->_map["CAP"] = &User::capCommand;
	this->_map["PASS"] = &User::authenticate;
	this->_map["NICK"] = &User::nickCommand;
	this->_map["USER"] = &User::userCommand;
	this->_map["QUIT"] = &User::quitCommand;
	this->_map["LUSERS"] = &User::lusersCommand;
}

User::~User(void)
{
}
/**
 * @brief it always firstly concatenates the old buffers 
 * 		contents stuck in 'inmsg' with the new buffer content.
 * 		Then launch the parsing and then send eventual messages
 * 		wrote in 'outmsg' to the remote.
 * 
 * @param buff : buffer filled on the reading step
 * @return true 
 * @return false 
 */
int		User::routine(std::string buff, size_t index)
{
	int		fdbk;

	fdbk = true;
	this->_inmsg += buff;
	if (this->_inmsg.find_first_of('\n', 0) != this->_inmsg.npos)
	{
std::cout << "inmsg: \"" + this->_inmsg << "\"" << std::endl;
		fdbk = parse();
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
		fdbk &= transmitMessage(index);
	}
	return (fdbk);
}

int	User::transmitMessage(const size_t& index)
{
	int		send_ret;

// std::cout << "before-----------" << this->_fd << "-------------- " << this->_user_socket->fd << std::endl;
	send_ret = send(Server::getSockfd(index), this->_outmsg.c_str(), \
		this->_outmsg.size(), MSG_DONTWAIT);
	if (send_ret < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
		return (Libftpp::error("send() would block"));
	if (send_ret < 0 && errno == EBADF)
	{
// std::cout << "after------------------------- " << this->_user_socket->fd << std::endl;
		// return (Libftpp::error("bad file"));
	}
	if (send_ret < 0)
		return (Libftpp::ft_perror("send()"));
// std::cout << "outmsg: " + this->_outmsg << std::endl;
	if ((size_t) send_ret < this->_outmsg.size())
	{
		this->_outmsg.erase(0, send_ret);
		return (true);
	}
	this->_outmsg.clear();
	return (true);
}

int	User::quitCommand()
{
	this->_outmsg += ErrorCommand(" ");
std::cout << this->_nickname << " quitted!" << std::endl;
	return (_fatal);
}

int	User::lusersCommand()
{
	if (!this->_registered)
		return (_ignore);
	this->_outmsg += numericMessage(HOSTNAME, RPL_LUSERCLIENT, \
		this->_nickname, "There are <u> users and <i> invisible on <s> servers");
	return (true);
}

int		User::lusers_rpl_numerics(void)
{
	size_t			n, m;
	std::string		msg;

	n = 0;
	m = 0;
	msg = "There are " + Libftpp::itoa(Server::getNbOfUsers()) + \
		" users and " + Libftpp::itoa(Server::getNbOfInvUsers()) + \
		" invisible on " + Libftpp::itoa(Server::getNbOfOtherServs()) + \
		" servers";
	this->_outmsg += numericMessage(HOSTNAME, RPL_LUSERCLIENT, \
		this->_nickname, msg);
	n = Server::getNbOfOperators();
	if (n < 2)
		msg = "operator online";
	else
		msg = "operators online";
	this->_outmsg += numericMessage(HOSTNAME, RPL_LUSEROP, \
		this->_nickname + " " + Libftpp::itoa(n), msg);
	n = Server::getNbOfUnkConnect();
	if (n < 2)
		msg = "unknown connection";
	else
		msg = "unknown connections";
	this->_outmsg += numericMessage(HOSTNAME, RPL_LUSERUNKNOWN, \
		this->_nickname + " " + Libftpp::itoa(n), msg);
	n = Server::getNbOfChannels();
	if (n < 2)
		msg = "channel formed";
	else
		msg = "channels formed";
	this->_outmsg += numericMessage(HOSTNAME, RPL_LUSERCHANNELS, \
		this->_nickname + " " + Libftpp::itoa(n), msg);
	msg = "I have " + Libftpp::itoa(Server::getNbOfUsers()) + \
		" clients and " + Libftpp::itoa(Server::getNbOfOtherServs()) + \
		" servers";
	this->_outmsg += numericMessage(HOSTNAME, RPL_LUSERME, \
		this->_nickname, msg);
	n = Server::getNbOfUsers();
	m = Server::getMaxOfUsers();
	msg = "Current local users " + Libftpp::itoa(n) + ", max " \
		+ Libftpp::itoa(m);
	this->_outmsg += numericMessage(HOSTNAME, RPL_LOCALUSERS, \
		this->_nickname + " " + Libftpp::itoa(n) + " " + Libftpp::itoa(m), \
		msg);
	n = Server::getNbOfUsers();
	m = Server::getMaxOfUsers();
	msg = "Current global users " + Libftpp::itoa(n) + ", max " \
		+ Libftpp::itoa(m);
	this->_outmsg += numericMessage(HOSTNAME, RPL_GLOBALUSERS, \
		this->_nickname + " " + Libftpp::itoa(n) + " " + Libftpp::itoa(m), \
		msg);
	return (true);
}

int		User::motd_rpl_numerics(void)
{
	std::string		filename;

	filename = Server::getMsgOfTheDay();
	if (filename.empty())
		this->_outmsg += numericMessage(HOSTNAME, ERR_NOMOTD, \
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
		this->_outmsg += numericMessage(HOSTNAME, ERR_NOMOTD, \
			this->_nickname, motd);
	}
	return (true);
}

int	User::parse()
{
	int					fdbk;
	std::string			line;
	std::istringstream	istr;

	fdbk = true;
// std::cout << "inmsg: '" + this->_inmsg + "'" << std::endl;
	Libftpp::trimStart(this->_inmsg, " \t\r\n");
	while (this->_inmsg.find_first_of('\n', 0) != this->_inmsg.npos)
	{
		this->_command.clear();
		this->_parameters.clear();
		line = Libftpp::extractStr(this->_inmsg, "\n");
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
				fdbk = (this->*_map.at(this->_command))();
				if (fdbk == _fatal)
					return (fdbk);
					// return (false);
			}
			catch(const std::exception& e)
			{
				std::cerr << "exception thrown in parse() " << e.what() << '\n';
				// return (true);
			}
// std::cerr << "command: " << this->_command << "\n";
// std::cerr << "parameters: " << this->_parameters << "\n";
		}
		// Libftpp::trimStart(this->_inmsg, " \t\r\n");
	}
	return (true);
}

std::string		User::serverMessage(std::string src, \
	std::string cmd, std::string msg)
{
	std::string	line;

	if (src.empty())
		src.assign("*");
	if (cmd.empty())
		cmd.assign("*");
	if (msg.empty())
		msg.assign(" ");
	line = ":" + src + " " + cmd + " :" + msg + "\r\n";
	return (line);
}
std::string		User::numericMessage(std::string src, \
	std::string num, std::string target, std::string msg)
{
	std::string	line;

	if (src.empty())
		src.assign("*");
	if (num.empty())
		num.assign("*");
	if (target.empty())
		target.assign("*");
	if (msg.empty())
		msg.assign(" ");
	line = ":" + src + " " + num + " " + target + " :" + msg + "\r\n";
	return (line);
}

std::string		User::ErrorCommand(std::string msg)
{
	std::string	line;

	if (msg.empty())
		msg.assign(" ");
	line = "ERROR :" + msg + "\r\n";
	return (line);
}