/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserTopicCommand.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 18:09:57 by jm                #+#    #+#             */
/*   Updated: 2024/05/10 18:11:48 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

// Command: TOPIC
// Parameters: <channel> [<topic>]
int		User::topicCommand(const size_t& index)
{
	int				fdbk;
	std::string		chann_name;
	std::string		topic;

	fdbk = true;
	if (!this->_registered)
	{
		this->_outmsg += Server::numericMessage(HOSTNAME, ERR_NOTREGISTERED, \
			this->_nickname, "you have not registered");
		return (_ignore);
	}
	Libftpp::trimStart(this->_parameters, " \t\r\n");
	chann_name = Libftpp::extractStr(this->_parameters, " \t\r\n", false);
	Libftpp::trimStart(this->_parameters, " \t\r\n");
	topic = this->_parameters;
	Libftpp::trim(topic, " \t\r\n");
	if (chann_name.empty())
	{
		this->_outmsg += Server::numericMessage(HOSTNAME, ERR_NEEDMOREPARAMS, \
			this->_nickname + " MODE", "Not enough parameters");
		return (_ignore);
	}
	try
	{
		return (Server::channels.at( \
			Libftpp::strToLower(chann_name)).topic(index, *this, topic));
	}
	catch(const std::exception& e)
	{
		// std::cerr << e.what() << '\n';
		this->_outmsg += Server::numericMessage(HOSTNAME, ERR_NOSUCHCHANNEL, \
			this->_nickname + " " + chann_name, "No such channel");
		return (_ignore);
	}
	return (fdbk);
}
