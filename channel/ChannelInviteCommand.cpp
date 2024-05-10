/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelInviteCommand.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 18:22:24 by jm                #+#    #+#             */
/*   Updated: 2024/05/10 18:22:57 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/**
 * @brief Checks if all is right before operating the INVITE process
 * 
 * @param index 
 * @param src 
 * @param target 
 * @return int 
 */
int		Channel::invite(const size_t& index, const std::string& src, const std::string& target)
{
	if (!isMember(index))
	{
		Server::getUser(index)->insertOutMessage(\
			Server::numericMessage(HOSTNAME, ERR_NOTONCHANNEL, \
			src + " " + this->_name, "You're not on that channel"));
		return (_ignore);
	}
	if (isInviteOnly() && \
		!this->_members.at(Server::getSockfd(index)).isOperator())
	{
		Server::getUser(index)->insertOutMessage(\
			Server::numericMessage(HOSTNAME, ERR_CHANOPRIVSNEEDED, \
			src + " " + this->_name, "You're not channel operator"));
		return (_ignore);
	}
	if (isMember(target))
	{
		Server::getUser(index)->insertOutMessage(\
			Server::numericMessage(HOSTNAME, ERR_USERONCHANNEL, \
			src + " " + target + " " + this->_name, "is already on channel"));
		return (_ignore);
	}
	return (invite_op(index, src, target));
}

/**
 * @brief operates the INVITE process
 * 
 * @param index 
 * @param src 
 * @param target 
 * @return int 
 */
int		Channel::invite_op(const size_t& index, const std::string& src, const std::string& target)
{
	User*	_user;

	_user = NULL;
	_user = Server::getUser(target);
	if (!_user)
	{
		Server::getUser(index)->insertOutMessage(\
			Server::numericMessage(HOSTNAME, ERR_NOSUCHNICK, \
			src + " " + target, "No such nick"));
		return (_ignore);
	}
	Server::getUser(index)->insertOutMessage(\
		(std::string) ":" + HOSTNAME + " " + RPL_INVITING + " " + src \
		+ " " + target + " " + this->_name);
	_user->insertOutMessage(\
		Server::serverMessage(src, "INVITE " + target + " " + this->_name));
	this->_invited.insert(target);
	return (true);
}
