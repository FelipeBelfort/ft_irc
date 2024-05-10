/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelprivmsgCommand.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 18:20:08 by jm                #+#    #+#             */
/*   Updated: 2024/05/10 18:20:44 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/**
 * @brief Sends the message to this channel members
 * 
 * @param index 
 * @param src 
 * @param targ 
 * @param msg 
 * @param only_to_opers 
 * @return int 
 */
int		Channel::privmsg(const size_t& index, const std::string& src, \
	const std::string& targ, std::string msg, bool only_to_opers)
{
	if (!isMember(index))
	{
		Server::getUser(index)->insertOutMessage(\
			Server::numericMessage(HOSTNAME, ERR_CANNOTSENDTOCHAN, \
			src + " " + this->_name, "Cannot send to channel"));
		return (true);
	}
	if (!msg.empty() && msg[0] == ':')
		Libftpp::trimStart(msg, ":", 1);
	else
		msg = Libftpp::extractStr(msg, " \t\r\n", false);
	for (std::map<int, Channel::Member>::iterator it = this->_members.begin(); \
		it != this->_members.end(); it++)
	{
		if ((*it).second.getName() == Libftpp::strToLower(src))
			continue ;
		try
		{
			if (only_to_opers)
			{
				if ((*it).second.isOperator())
					(*it).second.insertOutMessage(\
						Server::serverMessage(src, "PRIVMSG " + targ, msg));
			}
			else
			{
				(*it).second.insertOutMessage(\
					Server::serverMessage(src, "PRIVMSG " + targ, msg));
			}
		}
		catch(const std::exception& e)
		{
/*DEBUG*/	std::cerr << "Error: in Channel::privmsg(): " << e.what() << '\n';
			return (false);
		}
	}
	return (true);
}
