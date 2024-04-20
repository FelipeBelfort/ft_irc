/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 20:26:43 by TheTerror         #+#    #+#             */
/*   Updated: 2024/04/20 16:58:30 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User(pollfd &user_socket) : _user_socket(&user_socket), _fd(user_socket.fd), _registered(false)
{
	this->_map["CAP"] = &User::capCommand;
	this->_map["PASS"] = &User::authenticate;
	this->_map["NICK"] = &User::nickCommand;
	this->_map["USER"] = &User::userCommand;
	this->_map["QUIT"] = &User::quitCommand;
}

User::~User(void)
{
}
/**
 * @brief it always firstly concatenates the old buffers 
 * 		contents stuck in 'inmsg' with the new buffer content.
 * 		Then launch the parsing and then send eventual message
 * 		wrote in 'outmsg' to the remote.
 * 
 * @param buff : buffer filled on the reading step
 * @return true 
 * @return false 
 */
int		User::routine(std::string buff)
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
	if (!this->_outmsg.empty() && (this->_user_socket->revents & POLLOUT))
	{
		fdbk &= transmitMessage();
	}
	return (fdbk);
}

int	User::transmitMessage()
{
	int		send_ret;

// std::cout << "before-----------" << this->_fd << "-------------- " << this->_user_socket->fd << std::endl;
	send_ret = send(this->_user_socket->fd, this->_outmsg.c_str(), \
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

void	User::getMessage(std::string &msg)
{
	this->_inmsg += msg;
}

const std::string&	User::getNickname(void) const
{
	return (this->_nickname);
}
const std::string&	User::getOutMsg(void) const
{
	return (this->_outmsg);
}
bool				User::isRegistered(void) const
{
	return (this->_registered);
}

void				User::insertOutMessage(const std::string& msg)
{
	this->_outmsg += msg;
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