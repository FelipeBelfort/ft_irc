/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserGettersSetters.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 16:47:23 by jm                #+#    #+#             */
/*   Updated: 2024/04/21 17:05:14 by jm               ###   ########lyon.fr   */
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
