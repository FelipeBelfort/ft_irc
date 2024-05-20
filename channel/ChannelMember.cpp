/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelMember.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 19:31:28 by jm                #+#    #+#             */
/*   Updated: 2024/04/29 20:52:55 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Member::Member() : _index(0), _operator(false), _timestamp(std::time(NULL))
{
}

Channel::Member::Member(const size_t& index) : _index(index), _operator(false), _timestamp(std::time(NULL))
{
}

Channel::Member::~Member()
{
}

const size_t&		Channel::Member::getIndex(void) const
{
	return (this->_index);
}

const int&			Channel::Member::getSock_fd(void) const
{
	return (Server::getSockfd(this->_index));
}

/**
 * @brief looks up the User bound to actual index on the 'Server::_users'
 * 		map and returns reference on it
 * 
 * @return User& 
 * @exception throws UserNotFoundException when user lookup
 * 		according the known index fails
 */
User&		Channel::Member::getUser(void) const
{
	User*	_user;

	_user = NULL;
	_user = Server::getUser(this->_index);
	if (!_user)
		throw(UserNotFoundException());
	return (*_user);
}

const std::string&	Channel::Member::getName(void) const
{
	return (Server::getUser(this->_index)->getNickname());
}

bool				Channel::Member::isOperator(void) const
{
	return (this->_operator);
}

void				Channel::Member::setIndex(const size_t& id)
{
	this->_index = id;
}

void				Channel::Member::setOperator(bool b)
{
	this->_operator = b;
}

void				Channel::Member::insertOutMessage(const std::string& msg)
{
	getUser().insertOutMessage(msg);
}

const std::time_t&		Channel::Member::getTimestamp(void) const
{
	return (this->_timestamp);
}