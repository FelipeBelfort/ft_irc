/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 20:26:43 by TheTerror         #+#    #+#             */
/*   Updated: 2024/04/15 17:05:06 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User(pollfd &user_socket) : _user_socket(&user_socket), _is_logged(false)
{
	// this->_map["CAP"] = &User::authenticate;
	this->_map["PASS"] = &User::authenticate;
	this->_map["NICK"] = &User::nickCommand;
	this->_map["USER"] = &User::userCommand;
	this->_map["quitCommand"] = &User::quitCommand;
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
	int			send_ret;

	fdbk = true;
	this->_inmsg += buff;
	if (this->_inmsg.find_first_of('\n', 0) != this->_inmsg.npos)
	{
		fdbk = parse();
		if (!fdbk)
			return (fdbk);
		//parsing
	}
	else
		return (true); //TODO review the if/else
	
	if (!this->_outmsg.empty() && this->_user_socket->revents & POLLOUT)
	{
		send_ret = send(this->_user_socket->fd, this->_outmsg.c_str(), this->_outmsg.size(), 0);
		if (send_ret < 0)
			return (Libftpp::ft_perror("send()"));
std::cout << "outmsg: " + this->_outmsg << std::endl;
		if ((size_t) send_ret < this->_outmsg.size())
		{
			this->_outmsg.erase(0, send_ret);
			return (fdbk);
		}
		this->_outmsg.clear();
	}
	return (fdbk);
}

int	User::authenticate()
{
	std::string		first_param;

	this->_password.clear();
	this->_outmsg += "received pack: " + this->_command + ' ' + this->_parameters + "\r\n";
	Libftpp::trim(this->_parameters, " \t\n\r");
	if (this->_parameters[0] == ':')
	{
		Libftpp::trim(this->_parameters, ":", 1);
		first_param = this->_parameters;
	}
	else
	{
		first_param = Libftpp::extractStr(this->_parameters, " ");
		if (first_param.empty())
		{
			this->_outmsg += numericMessage("ircserver", ERR_PASSWDMISMATCH, \
				this->_nickname, "empty password");
			this->_outmsg += ErrorCommand(" ");
			return (_fatal);
		}
	}
	if (first_param == Server::_password)
	{
		this->_password.assign(first_param);
std::cout << "authentication: correct password" << std::endl;
	}
	else
	{
		this->_outmsg += numericMessage("ircserver", ERR_PASSWDMISMATCH, \
			this->_nickname, "incorrect password");
		this->_outmsg += ErrorCommand(" ");
{std::cout << "authentication: invalid password" << std::endl;}
		return (_fatal);
	}
	return (true);
}

int	User::nickCommand()
{
	this->_outmsg += "received pack: " + this->_command + ' ' + this->_parameters + "\r\n";
	Libftpp::trim(this->_parameters, " \t\n\r");
	this->_nickname = Libftpp::strToLower(this->_parameters);
std::cout << "nickname updated!" << std::endl;
	return (true);
}
int	User::userCommand()
{
	this->_outmsg += "received pack: " + this->_command + ' ' + this->_parameters + "\r\n";
	Libftpp::trim(this->_parameters, " \t\n\r");
	this->_username.assign(this->_parameters);
std::cout << "username updated!" << std::endl;
	this->_outmsg += numericMessage("ircserver", RPL_WELCOME, \
		this->_nickname, "Welcome to the 42IRC Network, " + \
		this->_nickname + "!jmf@localhost");
	return (true);
}

int	User::quitCommand()
{
	this->_outmsg += ErrorCommand(" ");
std::cout << this->_nickname << " exited!" << std::endl;
	return (_fatal);
}

int	User::parse()
{
	std::string			line;
	std::istringstream	istr;

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
				if ((this->*_map.at(this->_command))() == _fatal)
					return (_fatal);
					// return (false);
			}
			catch(const std::exception& e)
			{
				// std::cerr << "exception thrown in routine() " << e.what() << '\n';
				// return (true);
			}
// std::cerr << "command: " << this->_command << "\n";
// std::cerr << "parameters: " << this->_parameters << "\n";
		}
		Libftpp::trimStart(this->_inmsg, " \t\r\n");
	}
	return (true);
}

void	User::getMessage(std::string &msg)
{
	this->_inmsg += msg;
}

std::string	User::sendMessage(void)
{
	std::string	tmp = this->_inmsg;
	this->_inmsg.clear();
	return tmp;
}

const std::string	User::getNickname(void)
{
	return this->_nickname;
}

std::string		User::numericMessage(const std::string& src, \
	const std::string& num, const std::string& target, const std::string& msg)
{
	std::string	line;

	line = ":" + src + " " + num + " " + target + " :" + msg + "\r\n";
	return (line);
}

std::string		User::ErrorCommand(const std::string& msg)
{
	std::string	line;

	if (msg.empty())
		line = "ERROR : \r\n";
	else
		line = "ERROR :" + msg + "\r\n";
	return (line);
}