/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 22:01:11 by jm                #+#    #+#             */
/*   Updated: 2024/05/10 18:33:41 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(const std::string& name, const std::string& key)
	:_name(name), _key(key), _key_required(true), _invite_only(false), \
	_topic_is_locked(false), _limit_is_set(false), _limit(DEFAULT_LIMIT), \
	_isPionner_flag(true)
{
	std::time_t	tm;

	tm = std::time(NULL);
	this->_creationTimestamp.assign(Libftpp::itoa(tm));
	Libftpp::trim(this->_key, " \t\r\n");
	if (this->_key.empty())
	{
		_key_required = false;
/*DEBUG*/ std::cerr << "************constructor " << this->_key_required << " **************" << std::endl;

	}
}

Channel::~Channel()
{
	onChannelRemove();
}

const char*		Channel::UserNotFoundException::what(void) const throw()
{
	return ("user lookup failed");
}

/**
 * @brief Sends a message to all the members of the channel 
 * 		which amounts to send a message to the channel.
 * 		This function is like a template.
 * 
 * @param src 
 * @param cmd 
 * @param msg 
 * @return int 
 */
int		Channel::informMembers(const std::string& src, const std::string& cmd, \
	const std::string& msg)
{
	for (std::map<int, Channel::Member>::iterator it = this->_members.begin(); \
		it != this->_members.end(); it++)
	{
		// if ((*it).second.getName() == Libftpp::strToLower(src))
		// 	continue ;
		try
		{
			(*it).second.insertOutMessage(\
				Server::serverMessage(src, cmd, msg));
		}
		catch(const std::exception& e)
		{
/*DEBUG*/	std::cerr << "Error: in Channel::informMembers(): " << e.what() << '\n';
			return (false);
		}
	}
	return (true);
}
/**
 * @brief Sends a message to all the members of the channel 
 * 		which amounts to send a message to the channel.
 * 		This function is like a template.
 * 
 * @param src 
 * @param cmd 
 * @return int 
 */
int		Channel::informMembers(const std::string& src, const std::string& cmd)
{
	for (std::map<int, Channel::Member>::iterator it = this->_members.begin(); \
		it != this->_members.end(); it++)
	{
		// if ((*it).second.getName() == Libftpp::strToLower(src))
		// 	continue ;
		try
		{
			(*it).second.insertOutMessage(\
				Server::serverMessage(src, cmd));
		}
		catch(const std::exception& e)
		{
/*DEBUG*/	std::cerr << "Error: in Channel::informMembers(): " << e.what() << '\n';
			return (false);
		}
	}
	return (true);
}
