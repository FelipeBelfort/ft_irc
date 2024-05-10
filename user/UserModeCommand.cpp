/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserModeCommand.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 18:08:42 by jm                #+#    #+#             */
/*   Updated: 2024/05/10 18:08:58 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

// Command: MODE
// Parameters: <target> [<modestring> [<mode arguments>...]]
/**
 * @brief MODE command entry point.
 * 		Parses MODE parameters line, then launches accordingly 
 * 		either channel MODE part or user MODE part
 * 
 * @param index 
 * @return int 
 */
int		User::modeCommand(const size_t& index)
{
	int				fdbk;
	std::string		target;
	std::string		mode;

	fdbk = true;
	if (!this->_registered)
	{
		this->_outmsg += Server::numericMessage(HOSTNAME, ERR_NOTREGISTERED, \
			this->_nickname, "you have not registered");
		return (_ignore);
	}
	Libftpp::trimStart(this->_parameters, " \t\r\n");
	target = Libftpp::extractStr(this->_parameters, " \t\r\n", false);
	Libftpp::trimStart(this->_parameters, " \t\r\n");
	mode = this->_parameters;
	Libftpp::trim(mode, " \t\r\n");
	if (target.empty())
	{
		this->_outmsg += Server::numericMessage(HOSTNAME, ERR_NEEDMOREPARAMS, \
			this->_nickname + " MODE", "Not enough parameters");
		return (_ignore);
	}
	if (target[0] == '&' || target[0] == '#')
		return (channelMode(index, target, mode));
	else
		return (userMode(target, mode));
	return (fdbk);
}

/**
 * @brief Tries to find the channel in question 
 * 		to launch its MODE command handler
 * 
 * @param index 
 * @param target 
 * @param mode 
 * @return int 
 */
int		User::channelMode(const size_t& index, std::string& target, std::string& mode)
{
	try
	{
		return (Server::channels.at(Libftpp::strToLower(target)).mode(index, *this, mode));
	}
	catch(const std::exception& e)
	{
		// std::cerr << e.what() << '\n';
		this->insertOutMessage(\
			Server::numericMessage(HOSTNAME, ERR_NOSUCHCHANNEL, this->_nickname + \
			" " + target, "No such channel"));
		return (_ignore);
	}
	return (false);
}

/**
 * @brief user MODE command handler
 * 
 * @param target 
 * @param mode 
 * @return int 
 */
int		User::userMode(std::string& target, std::string& mode)
{
	std::string		rp_mode = mode;
	char			c;
	User*			_user;

	_user = NULL;
	_user = Server::getUser(Libftpp::strToLower(target));
	if (!_user)
	{
		insertOutMessage(\
			Server::numericMessage(HOSTNAME, ERR_NOSUCHNICK, this->_nickname + \
			" " + target, "No such nick"));
		return (_ignore);
	}
	if (_user->getNickname() != this->_nickname)
	{
		insertOutMessage(\
			Server::numericMessage(HOSTNAME, ERR_USERSDONTMATCH, this->_nickname, \
			"Cant change mode for other users"));
		return (_ignore);
	}
	if (mode.empty())
	{
		insertOutMessage(\
			Server::numericMessage(HOSTNAME, RPL_UMODEIS, this->_nickname + \
			" +" + this->_umode));
		return (_ignore);
	}
	for (size_t i = 0; i < mode.size(); i++)
	{
		c = mode[i];
		if (c == '+')
			setAUserMode(rp_mode, mode, ++i);
		else if (c == '-')
			removeAUserMode(rp_mode, mode, ++i);
		else
			Libftpp::removeChars(rp_mode, &c);
	}
	if (rp_mode.size() > 1)
		insertOutMessage(Server::serverMessage(this->_nickname, "MODE " + target, rp_mode));
	return (true);
}

int		User::setAUserMode(std::string& rp_mode, std::string& mode, size_t& i)
{
	char	c;

	for ( ; i < mode.size() && i != '+' && i != '-'; i++)
	{
		c = mode[i];
		if (c == 'o')
			Libftpp::removeChars(rp_mode, &c);
		else if (c == 'r')
			setUMode("r");
		else
		{
			Libftpp::removeChars(rp_mode, &c);
			insertOutMessage(\
				Server::numericMessage(HOSTNAME, ERR_UMODEUNKNOWNFLAG, getNickname(), \
				(std::string) "Unknown MODE flag " + c));
		}
	}
	return (true);
}

int		User::removeAUserMode(std::string& rp_mode, std::string& mode, size_t& i)
{
	char	c;

	for ( ; i < mode.size() && i != '+' && i != '-'; i++)
	{
		c = mode[i];
		if (c== 'o')
			setUMode("r");
		else if (c == 'r')
			setUMode("r");
		else
		{
			Libftpp::removeChars(rp_mode, &c);
			insertOutMessage(\
				Server::numericMessage(HOSTNAME, ERR_UMODEUNKNOWNFLAG, getNickname(), \
				(std::string) "Unknown MODE flag " + c));
		}
	}
	return (true);
}
