/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelModeCommand.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfaye <jfaye@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 18:23:57 by jm                #+#    #+#             */
/*   Updated: 2024/05/30 15:11:54 by jfaye            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/**
 * @brief 
 * Clean the string to trim all the duplicated whitespaces or signs
 * to avoid problems
 * 
 * @param str 
 */
static void	cleanMode(std::string& str)
{
	for (size_t pos_a = 0; pos_a != std::string::npos; pos_a = str.find_first_of("+-", pos_a + 1))
	{
		size_t	pos_b = str.find_first_not_of("-+", pos_a);
		str.replace(pos_a, pos_b - pos_a, str.substr(pos_b - 1, 1));
	}
	for (size_t pos_a = 0; pos_a != std::string::npos; pos_a = str.find_first_of(" \t", pos_a + 1))
	{
		size_t	pos_b = str.find_first_not_of(" \t", pos_a);
		str.replace(pos_a, pos_b - pos_a, " ");
	}
	Libftpp::trim(str, " \t\r\n");
}

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
	if (!this->_members.at(Server::getSockfd(index)).isOperator())
	{
		src.insertOutMessage( \
			Server::numericMessage(HOSTNAME, ERR_CHANOPRIVSNEEDED, \
			src.getNickname() + " " + this->_name, "You're not channel operator"));
		return (_ignore);
	}
	cleanMode(mode);
	return (applyModes(src, mode));
}

/**
 * @brief loops on mode changes requested launching 
 * 		accordingly either channel modes setter function 
 * 		or channel modes remover function.
 * 		In each loop it will call the rigth function to change each mode alone
 * 
 * @param src 
 * @param mode 
 * @return int 
 */
int		Channel::applyModes(User& src, std::string& mode)
{
	char		sign = 0;

	if (mode.find_first_of("+-") != 0 || mode.find_first_of("itkol") != 1)
	{
		src.insertOutMessage( \
				Server::numericMessage(HOSTNAME, ERR_UNKNOWNMODE, src.getNickname() \
				+ " " + mode[1], "is unknown mode char to me"));
		return (_ignore);
	}
	for (size_t i = mode.find_first_not_of("+-"); i < mode.size(); i = mode.find_first_not_of("+-"))
	{
		std::cout << "mode => |" << mode << "| char => |" << mode[i] << "|" << std::endl;
		if (mode[i - 1] && (mode[i - 1] == '+' || mode[i - 1] == '-'))
			sign = mode[i - 1];
		if (mode[i] == ' ' || !sign)
			return (true);
		else if (sign == '+')
		{
			setChannelModes(i, src, mode);
			continue;
		}
		else if (sign == '-')
		{
			removeChannelModes(i, src, mode);
			continue;
		}
		Libftpp::trim(mode, "\t\r\n");
	}
	return (true);
}

/**
 * @brief 
 * 	Look for the next argument into the string and then erases it from the source and returns the element found
 * 
 * @param str source to be looked into
 * @return std::string the argument OR empty if it doesn't exists
 */
std::string	Channel::getModeArgument(std::string& str)
{
	std::string	argument;
	size_t	pos = str.find_first_of(" \t\r\n");
		
	if (pos != std::string::npos)
	{
		argument = str.substr(pos, str.find_first_of(" \t\r\n", pos + 1) - pos);
		str.replace(str.find(argument), argument.size(), "");
		Libftpp::trim(argument, " \t\r\n");
	}

	return (argument);
}

/**
 * @brief sets channel modes referenced in 'mode' 
 * 
 * @param i actual index in modestring
 * @param src MODE command issuer
 * @param mode the modestring
 * @return int 
 */
int		Channel::setChannelModes(size_t& i, User& src, std::string& mode)
{
	std::string		argument;
	
	switch (mode[i])
	{

		case 'i':
		{
			setInviteOnly(true);
			break;
		}
		case 't':
		{
			setTopicProtection(true);
			break;
		}
		case 'l':
		{
			argument = getModeArgument(mode);
			if (argument.empty())
			{
				src.insertOutMessage( \
					Server::numericMessage(HOSTNAME, ERR_NEEDMOREPARAMS, \
					this->getName() + " MODE " + mode[i], "Not enough parameters"));
				return (mode.erase(i, 1), false);
			}
			if (!Libftpp::strIsInt(argument))
			{
				src.insertOutMessage( \
					Server::numericMessage(HOSTNAME, ERR_INVALIDMODEPARAM, \
					src.getNickname() + " " + this->getName() + " +" + mode[i] \
					+ " " + argument, "invalid argument"));
				return (mode.erase(i, 1), false);
			}
			setLimit(std::strtod(argument.c_str(), NULL));
			break;
		}
		case 'o':
		{
			argument = getModeArgument(mode);
			if (argument.empty())
			{
				src.insertOutMessage( \
					Server::numericMessage(HOSTNAME, ERR_NEEDMOREPARAMS, \
					this->getName() + " MODE " + mode[i], "Not enough parameters"));
				return (mode.erase(i, 1), false);
			}
			else if (!isMember(argument))
			{
				src.insertOutMessage(\
					Server::numericMessage(HOSTNAME, ERR_USERNOTINCHANNEL, \
					src.getNickname() + " " + argument + " " + this->_name, \
					"They aren't on that channel"));
			}
			else
				this->_members.at(Server::getSockfd(Server::getIndex(argument))).setOperator(true);
			break;
		}
		case 'k':
		{
			argument = getModeArgument(mode);
			if (argument.empty())
			{
				src.insertOutMessage( \
					Server::numericMessage(HOSTNAME, ERR_NEEDMOREPARAMS, \
					this->getName() + " MODE " + mode[i], "Not enough parameters"));
				return (mode.erase(i, 1), false); 
			}
			setKey(argument);
			break;
		}
		default:
		{
			src.insertOutMessage( \
				Server::numericMessage(HOSTNAME, ERR_UNKNOWNMODE, src.getNickname() \
				+ " " + mode[i], "is unknown mode char to me"));
			return (mode.erase(i, 1), false);
		}
	}
	argument.insert(0, "+ ");
	argument.insert(1, mode, i, 1);
	mode.erase(i, 1);
	return (informMembers(src.getNickname(), "MODE " + this->_name, argument));
}

/**
 * @brief removes channel modes referenced in 'mode'
 * 
 * @param i actual index in modestring
 * @param src MODE command issuer
 * @param mode the modestring
 * @return int 
 */
int		Channel::removeChannelModes(size_t& i, User& src, std::string& mode)
{
	std::string		argument;

	switch (mode[i])
	{
		case 'l':
		{
			unsetLimit();
			break;
		}
		case 'o':
		{
			argument = getModeArgument(mode);
			if (argument.empty())
			{
				src.insertOutMessage( \
					Server::numericMessage(HOSTNAME, ERR_NEEDMOREPARAMS, \
					this->getName() + " MODE " + mode[i], "Not enough parameters"));
				return (mode.erase(i, 1), false);
			}
			if (!isMember(argument))
			{
				src.insertOutMessage(\
					Server::numericMessage(HOSTNAME, ERR_USERNOTINCHANNEL, \
					src.getNickname() + " " + argument + " " + this->_name, \
					"They aren't on that channel"));
				return (mode.erase(i, 1), false);
			}
			this->_members.at(Server::getSockfd(Server::getIndex(argument))).setOperator(false);
			break;
		}
		case 'i':
		{
			unsetInviteOnly();
			break;
		}
		case 'k':
		{
			unsetKey();
			break;
		}
		case 't':
		{
			unsetTopicProtection();
			break;
		}
		default:
		{
			src.insertOutMessage( \
				Server::numericMessage(HOSTNAME, ERR_UNKNOWNMODE, src.getNickname() \
				+ " " + mode[i], "is unknown mode char to me"));
			return (mode.erase(i, 1), false);
		}
	}
	argument.insert(0, "- ");
	argument.insert(1, mode, i, 1);
	mode.erase(i, 1);

	return (informMembers(src.getNickname(), "MODE " + this->_name, argument));
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
