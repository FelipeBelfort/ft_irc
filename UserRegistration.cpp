/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserRegistration.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 18:46:11 by jm                #+#    #+#             */
/*   Updated: 2024/04/20 16:58:33 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

//TODO case CAP,PASS, NICK are sent yet and just after we try a PASS command

int	User::capCommand()
{
this->_outmsg += "received pack: " + this->_command + ' ' + this->_parameters + "\r\n";
	Libftpp::trim(this->_parameters, " \t\r\n");
	if (this->_parameters[0] == ':')
		Libftpp::trimStart(this->_parameters, ":", 1);
	this->_cap = this->_parameters;
	return (true);
}

int	User::authenticate()
{
	std::string		first_param;

	if (this->_registered)
	{
		this->_outmsg += numericMessage(HOSTNAME, ERR_ALREADYREGISTERED, \
			"", "You may not reregister");
		return(true);
	}
	if (this->_cap.empty())
	{
		this->_outmsg += ErrorCommand(\
            "Registration failure, sequence: CAP... - PASS... - NICK... - USER...");
		return(_fatal);
	}
	this->_password.clear();
this->_outmsg += "received pack: " + this->_command + ' ' + this->_parameters + "\r\n";
	Libftpp::trim(this->_parameters, " \t\n\r");
	if (this->_parameters[0] == ':')
	{
		Libftpp::trimStart(this->_parameters, ":", 1);
		first_param = this->_parameters;
	}
	else
		first_param = Libftpp::extractStr(this->_parameters, " \t");
	if (first_param.empty())
	{
		this->_outmsg += numericMessage(HOSTNAME, ERR_PASSWDMISMATCH, \
			"", "empty password");
		// this->_outmsg += ErrorCommand(" ");
		return (_fatal);
	}
	if (first_param == Server::_password)
	{
		this->_password.assign(first_param);
std::cout << "authentication: correct password" << std::endl;
	}
	else
	{
		this->_outmsg += numericMessage(HOSTNAME, ERR_PASSWDMISMATCH, \
			"", "incorrect password");
		// this->_outmsg += ErrorCommand(" ");
{std::cout << "authentication: invalid password" << std::endl;}
		return (_fatal);
	}
	return (true);
}

int	User::nickCommand()
{
	std::string		name;

	if (this->_password.empty())
	{
		this->_outmsg += ErrorCommand(\
			"Registration failure, sequence: CAP... - PASS... - NICK... - USER...");
		return(_fatal);
	}
	this->_outmsg += "received pack: " + this->_command + ' ' + this->_parameters + "\r\n";
	Libftpp::trim(this->_parameters, " \t\n\r");
	if (this->_parameters[0] == ':')
	{
		Libftpp::trimStart(this->_parameters, ":", 1);
		name = this->_parameters;
	}
	else
	{
		name = Libftpp::extractStr(this->_parameters, " \t");
		if (name.empty())
		{
			this->_outmsg += numericMessage(HOSTNAME, ERR_NONICKNAMEGIVEN, \
				"", "no nickname given");
			if (this->_registered)
				return (true);
			return (_fatal);
		}
	}
	if (!Server::isUniqueNick(name))
	{
		this->_outmsg += numericMessage(HOSTNAME, ERR_NICKNAMEINUSE, \
			"* " + name, "Nickname is already in use");
		if (this->_registered)
			return (_error);
		return (_fatal);
	}
	if (!Server::isValidNick(name))
	{
		this->_outmsg += numericMessage(HOSTNAME, ERR_ERRONEUSNICKNAME, \
			"* " + name, "Erroneus nickname");
		if (this->_registered)
			return (true);
		return (_fatal);
	}
	this->_oldnickname = this->_nickname;
	this->_nickname = Libftpp::strToLower(name);
	if (this->_registered)
		Server::broadcastMsg += serverMessage(this->_oldnickname, "NICK", \
			this->_nickname);
std::cout << "nickname updated!" << std::endl;
	if (!this->_username.empty() && !this->_registered)
		return (registrationComplete());
	return (true);
}

int	User::userCommand()
{
	std::string		username, hostname, servername, realname;

this->_outmsg += "received pack: " + this->_command + ' ' + this->_parameters + "\r\n";
	if (this->_registered || !this->_username.empty())
	{
		this->_outmsg += numericMessage(HOSTNAME, ERR_ALREADYREGISTERED, \
			this->_nickname, "You may not reregister");
		return(_error);
	}
	Libftpp::trim(this->_parameters, " \t\n\r");
	username = Libftpp::extractStr(this->_parameters, " \t");
	hostname = Libftpp::extractStr(this->_parameters, " \t");
	servername = Libftpp::extractStr(this->_parameters, " \t");
	realname = Libftpp::extractStr(this->_parameters, "");
	if (realname.empty())
	{
		this->_outmsg += numericMessage(HOSTNAME, ERR_NEEDMOREPARAMS, \
			this->_nickname + " USER", "Not enough parameters");
		return (_fatal);
	}
	this->_username = "~" + Libftpp::trim(username, " \t");
	this->_hostname = Libftpp::trim(hostname, " \t");
	this->_servername = Libftpp::trim(servername, " \t");
	realname = Libftpp::trimStart(realname, " \t");
	if (realname[0] == ':')
	{
		this->_realname = Libftpp::trimStart(realname, ":", 1);
	}
	else
	{
		this->_realname = Libftpp::extractStr(realname, " \t");
		if (this->_realname.empty())
		{
			this->_outmsg += numericMessage(HOSTNAME, ERR_NEEDMOREPARAMS, \
				this->_nickname + " USER", "Not enough parameters");
			return (_fatal);
		}
	}
std::cout << "username updated!" << std::endl;
	if (this->_nickname.empty())
	{
		// this->_outmsg += ErrorCommand(
		// 		"Registration failure, USER command failed, NICK command is required");
		return(_error);
	}
	return (registrationComplete());
}

int	User::registrationComplete()
{
	if (!this->_cap.empty() && !this->_password.empty() && \
		!this->_nickname.empty() && !this->_username.empty())
		this->_registered = true;
	else
		return (this->_registered = false, _fatal);
	this->_outmsg += numericMessage(HOSTNAME, RPL_WELCOME, \
		this->_nickname, "Welcome to the 42IRC Network, " + \
		this->_nickname + "!"+ this->_username + "@localhost");
	return (true);
}