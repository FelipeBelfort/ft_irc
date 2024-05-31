/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserRegistration.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfaye <jfaye@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 18:46:11 by jm                #+#    #+#             */
/*   Updated: 2024/05/31 17:45:31 by jfaye            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

//TODO case CAP,PASS, NICK are sent yet and just after we try a PASS command

// Command: CAP
// Parameters: <subcommand> [:<capabilities>]
/**
 * @brief CAP command processor.
 * 		1st step of the registration of a user.
 * 		Not useful, but stays mandatory.
 * 		The value of the parameter is not interpreted, just registered.
 * 
 * @param index 
 * @return int 
 */
int	User::capCommand(const size_t& index)
{
// this->_outmsg += "received pack: " + this->_command + ' ' + this->_parameters + "\r\n";
	(void)	index;
	Libftpp::trim(this->_parameters, " \t\r\n");
	if (this->_parameters[0] == ':')
		Libftpp::trimStart(this->_parameters, ":", 1);
	this->_cap = this->_parameters;
	return (true);
}

// Command: PASS
// Parameters: <password>
/**
 * @brief PASS command processor.
 * 		2nd step of the registration.
 * 		Parses parameters then authenticates 
 * 		the user's registration
 * 
 * @param index 
 * @return int 
 */
int	User::passCommand(const size_t& index)
{
	std::string		first_param;

	(void)	index;
	if (this->_registered)
	{
		this->_outmsg += Server::numericMessage(HOSTNAME, ERR_ALREADYREGISTERED, \
			"", "You may not reregister");
		return(true);
	}
	if (this->_cap.empty())
	{
		this->_outmsg += Server::ErrorMessage(\
            "Registration failure, sequence: CAP... - PASS... - NICK... - USER...");
		return(_fatal);
	}
	this->_password.clear();
	if (Server::_password.empty())
		return (true);
// /*DEBUG*/	this->_outmsg += "received pack: " + this->_command + ' ' + this->_parameters + "\r\n";
	Libftpp::trim(this->_parameters, " \t\n\r");
	if (this->_parameters[0] == ':')
	{
		Libftpp::trimStart(this->_parameters, ":", 1);
		first_param = this->_parameters;
	}
	else
		first_param = Libftpp::extractStr(this->_parameters, " \t", false);
	if (first_param.empty())
	{
		this->_outmsg += Server::numericMessage(HOSTNAME, ERR_PASSWDMISMATCH, \
			"", "empty password");
		// this->_outmsg += Server::ErrorMessage(" ");
		return (_fatal);
	}
	if (first_param == Server::_password)
	{
		this->_password.assign(first_param);
/*DEBUG*/	std::cout << "authentication: correct password" << std::endl;
	}
	else
	{
		this->_outmsg += Server::numericMessage(HOSTNAME, ERR_PASSWDMISMATCH, \
			"", "incorrect password");
		// this->_outmsg += Server::ErrorMessage(" ");
/*DEBUG*/	std::cout << "authentication: invalid password" << std::endl;
		return (_fatal);
	}
	return (true);
}

// Command: NICK
// Parameters: <nickname>
/**
 * @brief NICK command processor.
 * 		3rd step of the registration
 * 		parses parameters then if all is right sets 
 * 		the user's nickname which is not case-sensitive
 * 
 * @param index 
 * @return int 
 */
int	User::nickCommand(const size_t& index)
{
	std::string		name;

	(void)	index;
	if (this->_password.empty() && !Server::_password.empty())
	{
		this->_outmsg += Server::ErrorMessage(\
			"Registration failure, sequence-> CAP... - PASS... - NICK... - USER...");
		return(_fatal);
	}
// /*DEBUG*/ this->_outmsg += "received pack: " + this->_command + ' ' + this->_parameters + "\r\n";
	Libftpp::trim(this->_parameters, " \t\n\r");
	if (this->_parameters[0] == ':')
	{
		Libftpp::trimStart(this->_parameters, ":", 1);
		name = this->_parameters;
	}
	else
	{
		name = Libftpp::extractStr(this->_parameters, " \t", false);
		if (name.empty())
		{
			this->_outmsg += Server::numericMessage(HOSTNAME, ERR_NONICKNAMEGIVEN, \
				"", "no nickname given");
			if (this->_registered)
				return (true); 
			return (_fatal);
		}
	}
	if (!Server::isUniqueNick(name))
	{
		this->_outmsg += Server::numericMessage(HOSTNAME, ERR_NICKNAMEINUSE, \
			"* " + name, "Nickname is already in use");
		// if (this->_registered)
		return (_error);
		// return (_fatal);
	}
	if (!Server::isValidNick(name))
	{
		this->_outmsg += Server::numericMessage(HOSTNAME, ERR_ERRONEUSNICKNAME, \
			"* " + name, "Erroneus nickname");
		if (this->_registered)
			return (true);
		return (_fatal);
	}
	this->_oldnickname = this->_nickname;
	this->_nickname = Libftpp::strToLower(name);
	if (this->_registered)
		Server::broadcastMsg += Server::serverMessage(this->_oldnickname \
			+ "!" + this->_username + "@localhost", "NICK", \
			this->_nickname);
/*DEBUG*/ std::cout << "nickname updated!" << std::endl;
	if (!this->_username.empty() && !this->_registered)
		return (registrationComplete());
	return (true);
}

// Command: USER
// Parameters: <username> 0 * <realname>
/**
 * @brief USER command processor.
 * 		Final step of the registration.
 * 		Parses parameters then sets different informations related to the user.
 * 
 * @param index 
 * @return int 
 */
int	User::userCommand(const size_t& index)
{
	std::string		username, hostname, servername, realname;

	(void)	index;
// /*DEBUG*/ this->_outmsg += "received pack: " + this->_command + ' ' + this->_parameters + "\r\n";
	if (this->_registered || !this->_username.empty())
	{
		this->_outmsg += Server::numericMessage(HOSTNAME, ERR_ALREADYREGISTERED, \
			this->_nickname, "You may not reregister");
		return(_error);
	}
	if (this->_password.empty() && !Server::_password.empty())
	{
		this->_outmsg += Server::ErrorMessage(\
			"Registration failure, sequence-> CAP... - PASS... - NICK... - USER...");
		return(_fatal);
	}
	Libftpp::trim(this->_parameters, " \t\n\r");
	username = Libftpp::extractStr(this->_parameters, " \t", false);
	hostname = Libftpp::extractStr(this->_parameters, " \t", false);
	servername = Libftpp::extractStr(this->_parameters, " \t", false);
	realname = Libftpp::extractStr(this->_parameters, "", false);
	if (realname.empty())
	{
		this->_outmsg += Server::numericMessage(HOSTNAME, ERR_NEEDMOREPARAMS, \
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
		this->_realname = Libftpp::extractStr(realname, " \t", false);
		if (this->_realname.empty())
		{
			this->_outmsg += Server::numericMessage(HOSTNAME, ERR_NEEDMOREPARAMS, \
				this->_nickname + " USER", "Not enough parameters");
			return (_fatal);
		}
	}
/*DEBUG*/ std::cout << "username updated!" << std::endl;
	if (this->_nickname.empty())
	{
		// this->_outmsg += Server::ErrorMessage(
		// 		"Registration failure, USER command failed, NICK command is required");
		return(_error);
	}
	return (registrationComplete());
}

/**
 * @brief Upon successful completion, the Sever acknowledges the 
 * 		registration by issuing a sequence of messages to the user
 * 
 * @return int 
 */
int	User::registrationComplete(void)
{
	if (!this->_cap.empty() && \
		(!this->_password.empty() || (this->_password.empty() && Server::_password.empty())) && \
		!this->_nickname.empty() && !this->_username.empty())
		this->_registered = true;
	else
		return (this->_registered = false, _fatal);
	this->_outmsg += Server::numericMessage(HOSTNAME, RPL_WELCOME, \
		this->_nickname, "Welcome to the 42IRC Network, " + \
		this->_nickname + "!"+ this->_username + "@localhost");
	this->_outmsg += Server::numericMessage(HOSTNAME, RPL_YOURHOST, \
		this->_nickname, "Your host is " + \
		Server::sourcename + ", running version 0.0");
	this->_outmsg += Server::numericMessage(HOSTNAME, RPL_CREATED, \
		this->_nickname, "This server was created " + \
		Server::getStartDate());
	this->_outmsg += Server::numericMessage(HOSTNAME, RPL_MYINFO, \
		this->_nickname, Server::sourcename + " 0.0" + " rO" + " itkol");
	rpl_isupport_numerics();
	lusers_rpl_numerics();
	if (!motd_rpl_numerics())
		return (false);
	if (Server::isPionner_flag)
	{
		this->setUMode("o");
		Server::isPionner_flag = false;
	}
	else
		this->setUMode("r");
	this->_outmsg += (std::string) ":" + HOSTNAME + " " + RPL_UMODEIS + \
		" " + this->_nickname + " +" + this->_umode + "\r\n";
	Server::botSayHello(*this);
	return (true);
}

// parameters : "<client> <1-13 tokens> :are supported by this server"
/**
 * @brief the server issues RPL_ISUPPORT to advertise features to client
 * 
 * @return int 
 */
int		User::rpl_isupport_numerics(void)
{
	std::string	casemapping =	(std::string) "CASEMAPPING=" + CASEMAPPING;
	std::string	prefix = 		(std::string) "PREFIX=" + PREFIX;
	std::string	chantypes =		(std::string) "CHANTYPES=" + CHANTYPES;
	std::string	modes =			(std::string) "MODES=" + Libftpp::itoa(MODES);
	std::string	chanlimit =		(std::string) "CHANLIMIT=" + CHANLIMITPREFIXES + Libftpp::itoa(CHANLIMIT);
	std::string	nicklen =		(std::string) "NICKLEN=" + Libftpp::itoa(NICKLEN);
	std::string	topiclen =		(std::string) "TOPICLEN=" + Libftpp::itoa(TOPICLEN);
	std::string	kicklen =		(std::string) "KICKLEN=" + Libftpp::itoa(KICKLEN);
	std::string	channellen =	(std::string) "CHANNELLEN=" + Libftpp::itoa(CHANNELLEN);
	std::string	chanmodes =		(std::string) "CHANMODES=" + CHANMODES;
	std::string	userlen =		(std::string) "USERLEN=" + Libftpp::itoa(USERLEN);
	std::string	statusmsg =		(std::string) "STATUSMSG=" + STATUSMSG;

	this->_outmsg += Server::numericMessage(HOSTNAME, RPL_ISUPPORT, this->_nickname, \
		(std::string) casemapping + " " + prefix + " " + chantypes + " " \
		+ modes + " " + chanlimit + " " + nicklen + " " + topiclen + " " \
		+ kicklen + " " + channellen + " " + chanmodes + " " + userlen + " " \
		+ statusmsg + " :are supported by this server");
	return (true);
}

/**
 * @brief the Server issues statistics about local and global users
 * 
 * @return int 
 */
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
	this->_outmsg += Server::numericMessage(HOSTNAME, RPL_LUSERCLIENT, \
		this->_nickname, msg);
	n = Server::getNbOfOperators();
	if (n < 2)
		msg = "operator online";
	else
		msg = "operators online";
	this->_outmsg += Server::numericMessage(HOSTNAME, RPL_LUSEROP, \
		this->_nickname + " " + Libftpp::itoa(n), msg);
	n = Server::getNbOfUnkConnect();
	if (n < 2)
		msg = "unknown connection";
	else
		msg = "unknown connections";
	this->_outmsg += Server::numericMessage(HOSTNAME, RPL_LUSERUNKNOWN, \
		this->_nickname + " " + Libftpp::itoa(n), msg);
	n = Server::getNbOfChannels();
	if (n < 2)
		msg = "channel formed";
	else
		msg = "channels formed";
	this->_outmsg += Server::numericMessage(HOSTNAME, RPL_LUSERCHANNELS, \
		this->_nickname + " " + Libftpp::itoa(n), msg);
	msg = "I have " + Libftpp::itoa(Server::getNbOfUsers()) + \
		" clients and " + Libftpp::itoa(Server::getNbOfOtherServs()) + \
		" servers";
	this->_outmsg += Server::numericMessage(HOSTNAME, RPL_LUSERME, \
		this->_nickname, msg);
	n = Server::getNbOfUsers();
	m = Server::getMaxOfUsers();
	msg = "Current local users " + Libftpp::itoa(n) + ", max " \
		+ Libftpp::itoa(m);
	this->_outmsg += Server::numericMessage(HOSTNAME, RPL_LOCALUSERS, \
		this->_nickname + " " + Libftpp::itoa(n) + " " + Libftpp::itoa(m), \
		msg);
	n = Server::getNbOfUsers();
	m = Server::getMaxOfUsers();
	msg = "Current global users " + Libftpp::itoa(n) + ", max " \
		+ Libftpp::itoa(m);
	this->_outmsg += Server::numericMessage(HOSTNAME, RPL_GLOBALUSERS, \
		this->_nickname + " " + Libftpp::itoa(n) + " " + Libftpp::itoa(m), \
		msg);
	return (true);
}

