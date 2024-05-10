/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserInviteCommand.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 18:07:40 by jm                #+#    #+#             */
/*   Updated: 2024/05/10 18:07:54 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

// Command: INVITE
// Parameters: <nickname> <channel>
/**
 * @brief INVITE command entry point.
 * 		Parses INVITE parameters line 
 * 		and move on the INVITE process 
 * 		if all is right
 * 
 * @param index 
 * @return int 
 */
int		User::inviteCommand(const size_t& index)
{
	std::string	target;
	std::string	chann;

	if (!this->_registered)
	{
		this->_outmsg += Server::numericMessage(HOSTNAME, ERR_NOTREGISTERED, \
			this->_nickname, "you have not registered");
		return (_ignore);
	}
	Libftpp::trimStart(this->_parameters, " \t");
	target = Libftpp::extractStr(this->_parameters, " \t\r\n", false);
	Libftpp::trimStart(this->_parameters, " \t");
	chann = Libftpp::extractStr(this->_parameters, " \t\r\n", false);
	if (chann.empty())
	{
		this->_outmsg += Server::numericMessage(HOSTNAME, ERR_NEEDMOREPARAMS, \
			this->_nickname + " KICK", "Not enough parameters");
		return (_ignore);
	}
	return (tryInvite(index, target, chann));
}

/**
 * @brief try to find the channel in question to launch the INVITE process
 * 
 * @param index 
 * @param target 
 * @param chann 
 * @return int 
 */
int		User::tryInvite(const size_t& index, const std::string& target, std::string& chann)
{
	try
	{
		return (Server::channels.at( \
			Libftpp::strToLower(chann)).invite( \
			index, this->_nickname, target));
	}
	catch(const std::out_of_range& e)
	{
		// std::cerr << e.what() << '\n';
		this->_outmsg += Server::numericMessage(HOSTNAME, ERR_NOSUCHCHANNEL, \
			this->_nickname + " " + chann, "No such channel");
		return (_ignore);
	}
	catch(const std::exception& e)
	{
/*DEBUG*/std::cerr << "Fatal: User::tryInvite() failure: " << e.what() << '\n';
		return (false);
	}
	return (false);
}
