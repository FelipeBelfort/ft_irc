/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserOtherCommands.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 18:52:58 by jm                #+#    #+#             */
/*   Updated: 2024/05/10 18:12:23 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

/**
 * @brief The server acknowledges QUIT command by replying with an 
 * 		ERROR message and closing the connection to the client 
 * 		by returning '_fatal'
 * 
 * @param index 
 * @return _fatal 
 */
int		User::quitCommand(const size_t& index)
{
	(void)	index;
	this->_outmsg += Server::ErrorMessage(" ");
/*DEBUG*/std::cout << this->_nickname << " quitted!" << std::endl;
	return (_fatal);
}

int		User::lusersCommand(const size_t& index)
{
	(void)	index;
	if (!this->_registered)
		return (_ignore);
	this->_outmsg += Server::numericMessage(HOSTNAME, RPL_LUSERCLIENT, \
		this->_nickname, "There are <u> users and <i> invisible on <s> servers");
	return (true);
}

/**
 * @brief PING command processor.
 * 		Parses parameters then reply with PONG message
 * 
 * @param index 
 * @return int 
 */
int		User::pingCommand(const size_t& index)
{
	(void)	index;
	if (!this->_registered)
		return (_ignore);
	Libftpp::trim(this->_parameters, " \t");
	if (this->_parameters.empty())
		this->_outmsg += Server::numericMessage(HOSTNAME, ERR_NEEDMOREPARAMS, \
			this->_nickname + " PING", "Not enough parameters");
	this->_outmsg += (std::string) ":" + HOSTNAME + " PONG " + \
		this->_parameters + "\r\n";
	return (true);
}
