/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelGettersSetters.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 18:51:33 by jm                #+#    #+#             */
/*   Updated: 2024/05/10 14:07:11 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

bool	Channel::isInviteOnly(void) const
{
	return (this->_invite_only);
}

/**
 * @brief asserts membership
 * 
 * @param index index of the socket fd bound to 
 * 		the user which membership is being asserted
 * @return true 
 * @return false 
 */
bool	Channel::isMember(const size_t& index) const
{
	try
	{
		this->_members.at(Server::getSockfd(index));
	}
	catch(const std::exception& e)
	{
/*DEBUG*/std::cerr << "Channel::isMember() return false | " << e.what() << '\n';
		return (false);
	}
	return (true);
}

/**
 * @brief asserts membership
 * 
 * @param sockfd socket fd bound to 
 * 		the user which membership is being asserted
 * @return true 
 * @return false 
 */
bool	Channel::isMember(const int& sockfd) const
{
	try
	{
		this->_members.at(sockfd);
	}
	catch(const std::exception& e)
	{
/*DEBUG*/std::cerr << "Channel::isMember() return false | " << e.what() << '\n';
		return (false);
	}
	return (true);
}

/**
 * @brief asserts membership
 * 
 * @param nick nickname of the user which membership is being asserted
 * @return true 
 * @return false 
 */
bool	Channel::isMember(const std::string& nick)
{
	for (std::map<int, Channel::Member>::iterator it = this->_members.begin(); \
		it != this->_members.end(); it++)
	{
		if ((*it).second.getName() == Libftpp::strToLower(nick))
			return (true);
	}
	return (false);
}

bool	Channel::userIsInvited(const std::string& _username) const
{
	if (this->_invited.find(Libftpp::strToLower(_username)) != this->_invited.end())
		return (true);
	return (false);
}

const std::string&	Channel::getName(void) const
{
	return (this->_name);
}

const std::string	Channel::getTopic(void) const
{
	return (this->_topic);
}

bool			Channel::requireKey(void) const
{
/*DEBUG*/ std::cerr << "************ " << this->_key_required << " **************" << std::endl;
	return (this->_key_required);
}

bool			Channel::topicIsProtected(void) const
{
	return (_topic_is_locked);
}

const std::string	Channel::getKey(void) const
{
	return (this->_key);
}

const std::string	Channel::getCreationTime(void) const
{
	return (this->_creationTimestamp);
}

bool			Channel::isLimited(void) const
{
	return (this->_limit_is_set);
}

bool			Channel::topicIsSet(void) const
{
	if (this->_topic.empty())
		return (false);
	return (true);
}

const size_t&		Channel::getLimit(void) const
{
	return (this->_limit);
}

void				Channel::setKey(const std::string& key)
{
	this->_key_required = true;
	this->_key = key;
}


void				Channel::setTopicProtection(bool b)
{
	this->_topic_is_locked = b;
}

void				Channel::setInviteOnly(bool b)
{
	this->_invite_only = b;
}

void				Channel::setLimit(double limit)
{
	this->_limit_is_set = true;
	this->_limit = limit < 1 ? 0 : limit;
}

void				Channel::setTopic(const std::string& author, \
	const std::string& topic)
{
	time_t	tm;

	tm = std::time(NULL);
	this->_topic_author.assign(author);
	this->_topic_setat.assign(Libftpp::itoa(tm));
	this->_topic.assign(topic);
}

void				Channel::unsetKey(void)
{
	this->_key_required = false;
}

void				Channel::unsetLimit(void)
{
	this->_limit_is_set = false;
}

void				Channel::unsetInviteOnly(void)
{
	this->_invite_only = false;
}

void				Channel::unsetTopicProtection(void)
{
	this->_topic_is_locked = false;
}

void				Channel::clearTopic(void)
{
	this->_topic.clear();
}

/**
 * @brief removes USERNAME if found from the list of users 
 * 		marked as invited by the INVITE command
 * 
 * @param username 
 */
void				Channel::rmFromInvitedList(const std::string& username)
{
	this->_invited.erase(Libftpp::strToLower(username));
}
