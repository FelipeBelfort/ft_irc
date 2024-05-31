/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelOtherTools.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfaye <jfaye@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 18:30:28 by jm                #+#    #+#             */
/*   Updated: 2024/05/30 17:48:59 by jfaye            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

int		Channel::userForceQuit(const int& sockfd, const std::string& username)
{
	this->informMembers(username, "PART " + this->_name);
	this->_members.erase(sockfd);
	return (true);
}

int		Channel::onChannelRemove(void)
{
	for (std::map<int, Channel::Member>::iterator it = this->_members.begin(); \
		it != this->_members.end();  it++)
	{
		(*it).second.getUser().disjoinChannel(this->_name);
	}
	return (true);
}
