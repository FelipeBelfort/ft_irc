/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelTopicCommand.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 18:28:09 by jm                #+#    #+#             */
/*   Updated: 2024/05/10 18:28:37 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

int		Channel::topic(const size_t& index, User& src, std::string& topic)
{
	if (!isMember(index))
	{
		src.insertOutMessage( \
			Server::numericMessage(HOSTNAME, ERR_NOTONCHANNEL, \
			src.getNickname() + " " + this->_name, "You're not on that channel"));
		return (_ignore);
	}
	Libftpp::trim(topic, " \t\r\n");
	if (topic.empty())
		return (rpl_topic(src));
	if (topicIsProtected() && !this->_members.at(Server::getSockfd(index)).isOperator())
	{
		src.insertOutMessage( \
			Server::numericMessage(HOSTNAME, ERR_CHANOPRIVSNEEDED, \
			src.getNickname() + " " + this->_name, "You're not channel operator"));
		return (_ignore);
	}
	return (changeTopic(src, topic));
}

/**
 * @brief  Informs members of the current topic of the channel.
 * 
 * @param _user source
 * @return int 
 */
int		Channel::rpl_topic(User& _user)
{
	if (topicIsSet())
	{
		_user.insertOutMessage( \
			Server::numericMessage(HOSTNAME, RPL_TOPIC, _user.getNickname() \
			+ " " + this->_name, getTopic()));
		_user.insertOutMessage( \
			Server::numericMessage(HOSTNAME, RPL_TOPICWHOTIME, _user.getNickname() \
			+ " " + this->_name + " " + this->_topic_author + " " + this->_topic_setat));
	}
	else
	{
		_user.insertOutMessage( \
			Server::numericMessage(HOSTNAME, RPL_NOTOPIC, _user.getNickname() \
			+ " " + this->_name, "No topic is set"));
	}
	return (true);
}

int		Channel::changeTopic(User& src, std::string& topic)
{
	if (topic[0] == ':')
		topic.erase(topic.begin());
	if (topic.empty())
		clearTopic();
	else
		setTopic(src.getNickname(), topic);
	return (informMembers(src.getNickname(), "TOPIC " + this->_name, topic));
}
