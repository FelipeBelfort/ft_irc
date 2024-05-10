/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserKickCommand.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 18:06:21 by jm                #+#    #+#             */
/*   Updated: 2024/05/10 18:06:48 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

// Command: KICK
// Parameters: <channel> <user> *( "," <user> ) [<comment>]
/**
 * @brief entry point of KICK command.
 * 		Parses KICK parameters line
 * 
 * @param index 
 * @return int 
 */
int		User::kickCommand(const size_t& index)
{
	int			fdbk;
	std::string	chann;
	std::string	targets;
	std::string	comment;

	fdbk = true;
	if (!this->_registered)
	{
		this->_outmsg += Server::numericMessage(HOSTNAME, ERR_NOTREGISTERED, \
			this->_nickname, "you have not registered");
		return (_ignore);
	}
	Libftpp::trimStart(this->_parameters, " \t");
	chann = Libftpp::extractStr(this->_parameters, " \t\r\n", false);
	Libftpp::trimStart(this->_parameters, " \t");
	targets = Libftpp::extractStr(this->_parameters, " \t\r\n", false);
	comment = this->_parameters;
	Libftpp::trim(comment, " \t\r\n");
	if (targets.empty())
	{
		this->_outmsg += Server::numericMessage(HOSTNAME, ERR_NEEDMOREPARAMS, \
			this->_nickname + " KICK", "Not enough parameters");
		return (_ignore);
	}
	if (comment.empty())
		comment.assign("Unkown user");
	fdbk = tryKick(index, chann, targets, comment);
	return (fdbk);
}

/**
 * @brief looks up the channel in question then 
 * 		if exists launches the kick service
 * 
 * @param index 
 * @param chann name of the channel from which remove the target(s)
 * @param targets target(s)
 * @param comment eventual comment describing the kick
 * @return int 
 */
int		User::tryKick(const size_t& index, const std::string& chann, std::string& targets, std::string comment)
{
	try
	{
		return (Server::channels.at( \
			Libftpp::strToLower(chann)).kick( \
			index, this->_nickname, targets, comment));
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
/*DEBUG*/std::cerr << "Fatal: User::tryKick() failure: " << e.what() << '\n';
		return (false);
	}
	return (false);
}
