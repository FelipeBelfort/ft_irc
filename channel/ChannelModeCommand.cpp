/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelModeCommand.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 18:23:57 by jm                #+#    #+#             */
/*   Updated: 2024/05/10 18:26:33 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/**
 * @brief Checks beforehand that all is rightful to then proceed
 * 
 * @param index 
 * @param src 
 * @param mode 
 * @return int 
 */
int		Channel::mode(const size_t& index, User& src, std::string& mode)
{
	std::string		args;

	if (!isMember(index))
	{
		src.insertOutMessage( \
			Server::numericMessage(HOSTNAME, ERR_NOTONCHANNEL, \
			src.getNickname() + " " + this->_name, "You're not on that channel"));
		return (_ignore);
	}
	if (mode.empty())
	{
		rpl_channelModeIs(src);
		rpl_creationTime(src);
		return (_ignore);
	}
	args.assign(mode);
	mode = Libftpp::extractStr(args, " \t\r\n", false);
	Libftpp::trim(args, " \t\r\n");
	if (!this->_members.at(Server::getSockfd(index)).isOperator())
	{
		src.insertOutMessage( \
			Server::numericMessage(HOSTNAME, ERR_CHANOPRIVSNEEDED, \
			src.getNickname() + " " + this->_name, "You're not channel operator"));
		return (_ignore);
	}
	return (applyModes(src, mode, args));
}

/**
 * @brief loops on mode changes requested launching 
 * 		accordingly either channel modes setter function 
 * 		or channel modes remover function.
 * 		At the end, it issues a MODE message to the channel 
 * 		to inform members of the effective changes
 * 
 * @param src 
 * @param mode 
 * @param args 
 * @return int 
 */
int		Channel::applyModes(User& src, std::string& mode, std::string& args)
{
	std::string		rp_mode = mode; /**! ineffective mode characters will be 
									removed from this <modestring> to only keep 
									the effective ones that will be issued to the channel */
	std::string		rp_args; /**! used arguments will be added here to be included in the issued MODE message */
	char			c;

	for (size_t i = 0; i < mode.size(); i++)
	{
		c = mode[i];
		if (c == '+')
			setChannelModes(++i, src, mode, args, rp_mode, rp_args);
		else if (c == '-')
			removeChannelModes(++i, src, mode, args, rp_mode, rp_args);
		else
			Libftpp::removeChars(rp_mode, &c);
	}
	Libftpp::trim(rp_args, " ");
	if (rp_mode.size() > 1)
		return (informMembers(src.getNickname(), "MODE " + this->_name, rp_mode + " " + rp_args));
	return (true);
}

/**
 * @brief sets channel modes referenced in 'mode'
 * 
 * @param i actual index in modestring
 * @param src MODE command issuer
 * @param mode the modestring
 * @param args mode arguments
 * @param rp_mode effective modestring holder
 * @param rp_args effective arguments holder
 * @return int 
 */
int		Channel::setChannelModes(size_t& i, User& src, std::string& mode, \
	std::string& args, std::string& rp_mode, std::string& rp_args)
{
	std::string		argument;
	char			c;

	for ( ; i < mode.size(); i++)
	{
		c = mode[i];
		if (c == 'l')
		{
			argument = Libftpp::extractStr(args, " \t\r\n", false);
			if (argument.empty())
			{
				Libftpp::removeChars(rp_mode, &c);
				continue ;
			}
			if (!Libftpp::strIsInt(argument))
			{
				// args = argument + " " + args;
				continue ;
			}
			setLimit(std::strtod(argument.c_str(), NULL));
			rp_args.append(argument + " ");
		}
		else if (c == 'o')
		{
			argument = Libftpp::extractStr(args, " \t\r\n", false);
			if (argument.empty())
			{
				Libftpp::removeChars(rp_mode, &c);
				continue ;
			}
			if (!isMember(argument))
			{
				Libftpp::removeChars(rp_mode, &c);
				src.insertOutMessage(\
					Server::numericMessage(HOSTNAME, ERR_USERNOTINCHANNEL, \
					src.getNickname() + " " + argument + " " + this->_name, \
					"They aren't on that channel"));
				continue ;
			}
			this->_members.at(Server::getSockfd(Server::getIndex(argument))).setOperator(true);
			rp_args.append(argument + " ");
		}
		else if (c == 'i')
			setInviteOnly(true);
		else if (c == 'k')
		{
			argument = Libftpp::extractStr(args, " \t\r\n", false);
			Libftpp::trim(argument, " \t\r\n");
			if (argument.empty())
			{
				Libftpp::removeChars(rp_mode, &c);
				continue ;
			}
			setKey(argument);
			// rp_args.append(argument + " ");
		}
		else if (c == 't')
			setTopicProtection(true);
		else
		{
			Libftpp::removeChars(rp_mode, &c);
			src.insertOutMessage( \
				Server::numericMessage(HOSTNAME, ERR_UNKNOWNMODE, src.getNickname() \
				+ " " + c, "is unknown mode char to me"));
			continue ;
		}
	}
	return (true);
}

/**
 * @brief removes channel modes referenced in 'mode'
 * 
 * @param i actual index in modestring
 * @param src MODE command issuer
 * @param mode the modestring
 * @param args mode arguments
 * @param rp_mode effective modestring holder
 * @param rp_args effective arguments holder
 * @return int 
 */
int		Channel::removeChannelModes(size_t& i, User& src, std::string& mode, \
	std::string& args, std::string& rp_mode, std::string& rp_args)
{
	std::string		argument;
	char			c;

	for ( ; i < mode.size(); i++)
	{
		c = mode[i];
		if (c == 'l')
			unsetLimit();
		else if (c == 'o')
		{
			argument = Libftpp::extractStr(args, " \t\r\n", false);
			if (argument.empty())
			{
				Libftpp::removeChars(rp_mode, &c);
				continue ;
			}
			if (!isMember(argument))
			{
				Libftpp::removeChars(rp_mode, &c);
				src.insertOutMessage(\
					Server::numericMessage(HOSTNAME, ERR_USERNOTINCHANNEL, \
					src.getNickname() + " " + argument + " " + this->_name, \
					"They aren't on that channel"));
				continue ;
			}
			this->_members.at(Server::getSockfd(Server::getIndex(argument))).setOperator(false);
			rp_args.append(argument + " ");
		}
		else if (c == 'i')
			unsetInviteOnly();
		else if (c == 'k')
			unsetKey();
		else if (c == 't')
			unsetTopicProtection();
		else
		{
			Libftpp::removeChars(rp_mode, &c);
			src.insertOutMessage( \
				Server::numericMessage(HOSTNAME, ERR_UNKNOWNMODE, src.getNickname() \
				+ " " + mode[i], "is unknown mode char to me"));
		}
	}
	return (true);
}

int		Channel::rpl_channelModeIs(User& _user)
{
	std::string		modes;
	std::string		args;

	modes.assign("+");
	if (isLimited())
	{
		modes += "l";
		args += Libftpp::itoa(getLimit());
	}
	if (isInviteOnly())
		modes += "i";
	if (requireKey())
		modes += "k";
	if (topicIsProtected())
		modes += "t";
	if (modes.size() == 1)
		modes.clear();
	_user.insertOutMessage( \
		Server::numericMessage(HOSTNAME, RPL_CHANNELMODEIS, _user.getNickname() \
		+ " " + this->_name + " " + modes + " " + args));
	return (true);
}

int		Channel::rpl_creationTime(User& _user)
{
	_user.insertOutMessage( \
		Server::numericMessage(HOSTNAME, RPL_CREATIONTIME, _user.getNickname() \
		+ " " + this->_name + " " + getCreationTime()));
	return (true);
}
