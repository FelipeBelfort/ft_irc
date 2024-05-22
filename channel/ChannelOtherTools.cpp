/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelOtherTools.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 18:30:28 by jm                #+#    #+#             */
/*   Updated: 2024/05/10 18:32:20 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

int		Channel::userForceQuit(const int& sockfd, const std::string& username)
{
	this->_members.erase(sockfd);
	this->informMembers(username, "PART " + this->_name);
	// if (!this->getNbOfMembers())
	// 	return (Server::removeChannel(this->_name), true);
	return (true);
}

int		Channel::onChannelRemove(void)
{
// /*DEBUG*/std::cerr << "rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr" << '\n';
	for (std::map<int, Channel::Member>::iterator it = this->_members.begin(); \
		it != this->_members.end();  it++)
	{
		(*it).second.getUser().disjoinChannel(this->_name);
	}
	return (true);
}
