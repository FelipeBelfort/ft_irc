/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserGettersSetters.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 16:47:23 by jm                #+#    #+#             */
/*   Updated: 2024/05/10 00:07:58 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

/**
 * @brief returns user's nickname
 * 
 * @return const std::string& 
 */
const std::string&	User::getNickname(void) const
{
	return (this->_nickname);
}

/**
 * @brief returns user's out message buffer
 * 
 * @return const std::string& 
 */
const std::string&	User::getOutMsg(void) const
{
	return (this->_outmsg);
}

/**
 * @brief asserts whether the user is registered or not
 * 
 * @return true 
 * @return false 
 */
bool				User::isRegistered(void) const
{
	return (this->_registered);
}

bool				User::isoperator(void) const
{
	if (this->_umode.find('o', 0) != this->_umode.npos)
		return (true);
	return (false);
}

bool				User::isRegUser(void) const
{
	if (this->_umode.find('r', 0) != this->_umode.npos && !this->isoperator())
		return (true);
	return (false);
}

void				User::setUMode(const std::string& newmode)
{
	this->_umode.assign(newmode);
}

/**
 * @brief sets the user's output message buffer by appending 
 * 		the new message 'msg' to it
 * 
 * @param msg new message to append
 */
void				User::insertOutMessage(const std::string& msg)
{
	this->_outmsg += msg;
}

/**
 * @brief Adds the channel named CHANN_NAME to the list of 
 * 		joined channels of this user.
 * 		By this way, a user aknowledges his membership to a channel.
 * 
 * @param chann_name name of the channel in question
 */
void				User::newJoinedChannel(const std::string& chann_name)
{
	this->_joinedchannels.insert(Libftpp::strToLower(chann_name));
}

bool				User::canJoinAChannel(const std::string& chann_name)
{
	if (this->_joinedchannels.size() >= CHANLIMIT)
	{
		this->_outmsg += Server::numericMessage(HOSTNAME, ERR_TOOMANYCHANNELS, \
			this->_nickname + " " + chann_name, "You have joined too many channels");
		return (false);
	}
	return (true);
}
