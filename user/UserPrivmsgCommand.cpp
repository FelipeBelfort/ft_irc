/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserPrivmsgCommand.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 18:04:27 by jm                #+#    #+#             */
/*   Updated: 2024/05/10 18:05:57 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

// Command: PRIVMSG
// Parameters: <target>{,<target>} <text to be sent>
/**
 * @brief Checks if the parameters pattern is rightful and
 * 		then launches the identification of the targets (user or channel)
 * 		with identifyTarget();
 * 
 * @param index 
 * @return int 
 */
int		User::privmsgCommand(const size_t& index)
{
	int				fdbk;
	std::string		targets;
	std::string		msg;

	fdbk = true;
	if (!this->_registered)
	{
		this->_outmsg += Server::numericMessage(HOSTNAME, ERR_NOTREGISTERED, \
			this->_nickname, "you have not registered");
		return (_ignore);
	}
	targets = Libftpp::extractStr(this->_parameters, " \t\r\n", false);
	msg = this->_parameters;
	Libftpp::trim(targets, " \t\r\n");
	Libftpp::trim(msg, " \t\r\n");
	if (targets.empty())
	{
		this->_outmsg += Server::numericMessage(HOSTNAME, ERR_NORECIPIENT, \
			this->_nickname, "No recipient given PRIVMSG");
		return (_ignore);
	}
	if (msg.empty())
	{
		this->_outmsg += Server::numericMessage(HOSTNAME, ERR_NOTEXTTOSEND, \
			this->_nickname, "No text to send");
		return (_ignore);
	}
	fdbk = identifyTarget(index, targets, msg);
	return (fdbk);
}

/**
 * @brief Loops on TARGETS and for each target identifies 
 * 		the type (user or channel) to launch the respective part
 * 
 * @param index 
 * @param targets 
 * @param msg 
 * @return int 
 */
int		User::identifyTarget(const size_t& index, std::string& targets, const std::string& msg)
{
	int			fdbk;
	bool		only_to_opers;

	only_to_opers = false;
	for (std::string targ; !targets.empty(); )
	{
		targ.assign(Libftpp::extractStr(targets, ",\r\n", false));
		if (targ[0] == '@')
		{
			only_to_opers = true;
			Libftpp::trimStart(targ, "@", 1);
		}
		if (!targ.empty() && (targ[0] == '&' || targ[0] == '#'))
		{
			fdbk = tryTargetAsChannel(index, targ, msg, only_to_opers);
			if (!fdbk)
				return (false);
			if (fdbk == true)
				continue ;
		}
		fdbk = tryTargetAsUser(targ, msg);
		if (!fdbk)
			return (false);
		if (fdbk == true)
			continue ;
	}
	return (true);
}

/**
 * @brief Searches firstly the targeted channel, if found it launches 
 * 		its privmsg service (privmsg())
 * 
 * @param index 
 * @param targ 
 * @param msg 
 * @param only_to_opers to target the channel operators only
 * @return int 
 */
int		User::tryTargetAsChannel(const size_t& index, std::string& targ, \
	const std::string& msg, bool only_to_opers)
{
	for (std::map<std::string, Channel>::iterator it = Server::channels.begin(); \
		it != Server::channels.end(); it++)
	{
		if (Libftpp::strToLower((*it).second.getName()) == Libftpp::strToLower(targ))
		{
			return ((*it).second.privmsg(index, this->_nickname, targ, msg, only_to_opers));
		}
	}
	this->_outmsg += Server::numericMessage(HOSTNAME, ERR_NOSUCHCHANNEL, \
		this->_nickname + " " + targ, "No such channel");
	return (_ignore);
}

/**
 * @brief Searches firstly the targeted user, if found it launches 
 * 		its privmsg service (privmsgTowardsUser())
 * 
 * @param targ 
 * @param msg 
 * @return int 
 */
int		User::tryTargetAsUser(std::string& targ, const std::string& msg)
{
	User*	_user;

	for (size_t i = 1; i < Server::getSocketsSize(); i++)
	{
		_user = NULL;
		_user = Server::getUser(i);
		if (!_user)
			return (false);
		if (Libftpp::strToLower(_user->getNickname()) == Libftpp::strToLower(targ))
		{
			if (!privmsgTowardsUser(_user, msg))
				return (false);
			return (true);
		}
	}
	this->_outmsg += Server::numericMessage(HOSTNAME, ERR_NOSUCHNICK, \
		this->_nickname + " " + targ, "No such nick");
	return (_ignore);
}

/**
 * @brief sends the message to the targeted user
 * 
 * @param user_targeted 
 * @param msg 
 * @return int 
 */
int		User::privmsgTowardsUser(User* user_targeted, std::string msg)
{
	if (msg[0] == ':')
	{
		Libftpp::trimStart(msg, ":", 1);
		user_targeted->insertOutMessage(\
			Server::serverMessage(this->_nickname, "PRIVMSG " + \
				user_targeted->getNickname(), msg));
		return (true);
	}
	else
	{
		msg = Libftpp::extractStr(msg, " :\t\r\n", false);
		user_targeted->insertOutMessage(\
			Server::serverMessage(this->_nickname, "PRIVMSG " + \
				user_targeted->getNickname(), msg));
		return (true);
	}
	return (true);
}
