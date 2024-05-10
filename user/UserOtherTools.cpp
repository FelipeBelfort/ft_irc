/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserOtherTools.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 17:58:51 by jm                #+#    #+#             */
/*   Updated: 2024/05/10 17:59:37 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

int		User::disjoinChannel(const std::string& chann_name)
{
	this->_joinedchannels.erase(Libftpp::strToLower(chann_name));
/*DEBUG*/std::cerr << "Success: disjoinChannel(): channel " << chann_name << " erased from " << this->_nickname << '\n';
	return (true);
}

int		User::onUserRemove(void)
{
	for (std::set<std::string>::iterator it = this->_joinedchannels.begin(); \
		it != this->_joinedchannels.end(); it++)
	{
		try
		{
			Server::channels.at(*it).userForceQuit(this->_sockfd, this->_nickname);
		}
		catch(const std::exception& e)
		{
/*DEBUG*/	std::cerr << "Fatal: in User class destructor: onUserRemove() failed: " << e.what() << '\n';
		}
	}
/*DEBUG*/std::cerr << "success: onUserRemove() succeeded" << '\n';
	return (true);
}
