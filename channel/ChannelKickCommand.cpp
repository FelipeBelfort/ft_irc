/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelKickCommand.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 18:21:21 by jm                #+#    #+#             */
/*   Updated: 2024/05/10 18:21:47 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/**
 * @brief checks beforehand if the user requester is rightful enough.
 * 		If all is right, loops on targets and kick them from the channel
 * 
 * @param index 
 * @param src 
 * @param targets 
 * @param comment 
 * @return int 
 */
int		Channel::kick(const size_t& index, const std::string& src, \
	std::string targets, const std::string& comment)
{
	int		fdbk;

	fdbk = true;
	if (!isMember(index))
	{
		Server::getUser(index)->insertOutMessage(\
			Server::numericMessage(HOSTNAME, ERR_NOTONCHANNEL, \
			src + " " + this->_name, "You're not on that channel"));
		return (_ignore);
	}
	if (!this->_members.at(Server::getSockfd(index)).isOperator())
	{
		Server::getUser(index)->insertOutMessage(\
			Server::numericMessage(HOSTNAME, ERR_CHANOPRIVSNEEDED, \
			src + " " + this->_name, "You're not channel operator"));
		return (_ignore);
	}
	for (std::string targ; !targets.empty(); )
	{
		targ.assign(Libftpp::extractStr(targets, ",\r\n", false));
		fdbk = kick_op(src, targ, comment);
		if (!fdbk)
			return (false);
		if (fdbk != true)
			Server::getUser(index)->insertOutMessage(\
				Server::numericMessage(HOSTNAME, ERR_USERNOTINCHANNEL, \
				src + " " + targ + " " + this->_name, "He isn't on that channel"));
	}
	return (true);
}

/**
 * @brief looks up TARG among members and if found, kicks him from this channel
 * 
 * @param src 
 * @param targ 
 * @param comment 
 * @return int 
 */
int		Channel::kick_op(const std::string& src, const std::string& targ, \
	const std::string& comment)
{
	for (std::map<int, Channel::Member>::iterator it = this->_members.begin(); \
		it != this->_members.end(); it++)
	{
		if ((*it).second.getName() == Libftpp::strToLower(targ))
		{
			int	fdbk = informMembers(src, "KICK " + this->_name + " " + targ, comment);
			(*it).second.getUser().disjoinChannel(this->_name);
			this->_members.erase((*it).first);
			if (!fdbk)
				return (false);
			return (true);
		}
	}
	return (_ignore);
}
