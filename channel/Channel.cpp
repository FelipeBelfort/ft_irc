/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 22:01:11 by jm                #+#    #+#             */
/*   Updated: 2024/05/10 15:38:35 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(const std::string& name, const std::string& key)
	:_name(name), _key(key), _key_required(true), _invite_only(false), \
	_topic_is_locked(false), _limit_is_set(false), _limit(DEFAULT_LIMIT), \
	_isPionner_flag(true)
{
	std::time_t	tm;

	tm = std::time(NULL);
	this->_creationTimestamp.assign(Libftpp::itoa(tm));
	Libftpp::trim(this->_key, " \t\r\n");
	if (this->_key.empty())
	{
		_key_required = false;
/*DEBUG*/ std::cerr << "************constructor " << this->_key_required << " **************" << std::endl;

	}
}

Channel::~Channel()
{
	onChannelRemove();
}

// Command: JOIN
// Parameters: <channel>{,<channel>} [<key>{,<key>}]
// Alt Params: 0
/**
 * @brief Checks if it is actually rightful to continue 
 * 		joining the user to this channel
 * 
 * @param id 
 * @param key eventual channel keyword provided by the user
 * @return int 
 */
int	Channel::joinChannel(const size_t& id, const std::string& key)
{
	int			fdbk;
	Member		new_memb(id);
	User*		_user;
	std::string	username;

	fdbk = true;
	_user = Server::getUser(id);
	if (!_user)
		return (false);
	username = _user->getNickname();
	fdbk = isNotMember(Server::getSockfd(id));
	if (fdbk != true)
		return (fdbk);
	fdbk = canJoinTheChannel(_user, username, key);
	if (fdbk != true)
		return (fdbk);
	if (this->_isPionner_flag)
	{
		new_memb.setOperator(true);
		this->_isPionner_flag = false;
	}
	syncInsertion(id, new_memb);
	acknowledgementSequence(_user, username);
	return (true);
}

/**
 * @brief asserts if the user corresponding to 'sockfd' is not member of this channel 
 * 
 * @param sockfd 
 * @return int 
 */
int		Channel::isNotMember(const int& sockfd)
{
	try
	{
		if (this->_members.at(sockfd).getSock_fd() == sockfd)
			return (_ignore);
		else
			std::cerr << "Warning!!!: isNotMember() failed" << '\n';
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error: isNotMember(): " << e.what() << '\n';
	}
	return (true);
}

/**
 * @brief asserts if the user is rightful joining this channel
 * 
 * @param _user pointer to user joining the channel
 * @param username 
 * @param key channel access keyword
 * @return int 
 */
int	Channel::canJoinTheChannel(User* _user, const std::string& username, \
	const std::string& key)
{
	if (!_user->canJoinAChannel(this->_name))
		return (_error);
	if (requireKey() && key != this->_key)
	{
/*DEBUG*/std::cerr << "ùùùùùùùùù" << std::endl;
		_user->insertOutMessage(Server::numericMessage(HOSTNAME, \
			ERR_BADCHANNELKEY, username + " " + this->_name, \
			"Cannot join channel (+k)"));
		return (_error);
	}
	if (this->_limit_is_set)
	{
		if (this->_members.size() >= this->_limit)
		{
			_user->insertOutMessage(Server::numericMessage(HOSTNAME, \
				ERR_CHANNELISFULL, username + " " + this->_name, \
				"Cannot join channel (+l)"));
			return (_error);
		}
	}
	if (isInviteOnly() && !userIsInvited(username))
	{
		_user->insertOutMessage(Server::numericMessage(HOSTNAME, \
			ERR_INVITEONLYCHAN, username + " " + this->_name, \
			"Cannot join channel (+i)"));
		return (_error);
	}
	return (true);
}

/**
 * @brief Synchronizes the configuration of a user’s membership on this channel.
 * 		On one hand by adding the user on the list of members and on the other hand
 * 		by making the user aknowledge that he is member on that channel.
 * 
 * @param id index of the user's socket in sockets container
 * @param new_memb Instance of Member class corresponding to the user
 * @return int 
 */
int		Channel::syncInsertion(const size_t& id, const Member& new_memb)
{
	this->_members.insert(std::pair<int, Member>(Server::getSockfd(id), new_memb));
	this->_members.at(Server::getSockfd(id)).getUser().newJoinedChannel(this->_name);
	return (false);
}

/**
 * @brief Server aknowledges the user's membership by issuing 
 * 		the sequence of messages updating the members
 * 
 * @param _user 
 * @param username 
 * @return int 
 */
int		Channel::acknowledgementSequence(User* _user, const std::string& username)
{
	informMembers(username, "JOIN", this->_name);
	rpl_topic(*_user);
	// if (topicIsSet())
	// {
	// 	_user->insertOutMessage(Server::numericMessage(HOSTNAME, 
	// 		RPL_TOPIC, username + " " + this->_name, this->_topic));
	// 	_user->insertOutMessage(Server::numericMessage(HOSTNAME, 
	// 		RPL_TOPICWHOTIME, username + " " + this->_name + " " 
	// 		+ this->_topic_author, this->_topic_setat));
	// 	// return (_ignore);
	// }
	for (std::map<int, Channel::Member>::iterator it = this->_members.begin(); \
		it != this->_members.end(); it++)
	{
		if ((*it).second.isOperator())
			_user->insertOutMessage(Server::numericMessage(HOSTNAME, \
				RPL_NAMREPLY, username + " = " + this->_name, \
				"@" + (*it).second.getName()));
		else
			_user->insertOutMessage(Server::numericMessage(HOSTNAME, \
				RPL_NAMREPLY, username + " = " + this->_name, \
				(*it).second.getName()));
	}
	_user->insertOutMessage(Server::numericMessage(HOSTNAME, \
		RPL_ENDOFNAMES, username + " " + this->_name, \
		"End of /NAMES list"));
	rmFromInvitedList(username);
	return (true);
}

/**
 * @brief Sends the message to this channel members
 * 
 * @param index 
 * @param src 
 * @param targ 
 * @param msg 
 * @param only_to_opers 
 * @return int 
 */
int		Channel::privmsg(const size_t& index, const std::string& src, \
	const std::string& targ, std::string msg, bool only_to_opers)
{
	if (!isMember(index))
	{
		Server::getUser(index)->insertOutMessage(\
			Server::numericMessage(HOSTNAME, ERR_CANNOTSENDTOCHAN, \
			src + " " + this->_name, "Cannot send to channel"));
		return (true);
	}
	if (!msg.empty() && msg[0] == ':')
		Libftpp::trimStart(msg, ":", 1);
	else
		msg = Libftpp::extractStr(msg, " \t\r\n", false);
	for (std::map<int, Channel::Member>::iterator it = this->_members.begin(); \
		it != this->_members.end(); it++)
	{
		if ((*it).second.getName() == Libftpp::strToLower(src))
			continue ;
		try
		{
			if (only_to_opers)
			{
				if ((*it).second.isOperator())
					(*it).second.insertOutMessage(\
						Server::serverMessage(src, "PRIVMSG " + targ, msg));
			}
			else
			{
				(*it).second.insertOutMessage(\
					Server::serverMessage(src, "PRIVMSG " + targ, msg));
			}
		}
		catch(const std::exception& e)
		{
/*DEBUG*/	std::cerr << "Error: in Channel::privmsg(): " << e.what() << '\n';
			return (false);
		}
	}
	return (true);
}

/**
 * @brief checks beforehand if the user requester is rightful enough.
 * 		If all is right, loops on targets and kick them from the channel
 * 
 * @param index 
 * @param src 
 * @param targets 
 * @param comment 
 * @return int 
 */
int		Channel::kick(const size_t& index, const std::string& src, \
	std::string targets, const std::string& comment)
{
	int		fdbk;

	fdbk = true;
	if (!isMember(index))
	{
		Server::getUser(index)->insertOutMessage(\
			Server::numericMessage(HOSTNAME, ERR_NOTONCHANNEL, \
			src + " " + this->_name, "You're not on that channel"));
		return (_ignore);
	}
	if (!this->_members.at(Server::getSockfd(index)).isOperator())
	{
		Server::getUser(index)->insertOutMessage(\
			Server::numericMessage(HOSTNAME, ERR_CHANOPRIVSNEEDED, \
			src + " " + this->_name, "You're not channel operator"));
		return (_ignore);
	}
	for (std::string targ; !targets.empty(); )
	{
		targ.assign(Libftpp::extractStr(targets, ",\r\n", false));
		fdbk = kick_op(src, targ, comment);
		if (!fdbk)
			return (false);
		if (fdbk != true)
			Server::getUser(index)->insertOutMessage(\
				Server::numericMessage(HOSTNAME, ERR_USERNOTINCHANNEL, \
				src + " " + targ + " " + this->_name, "He isn't on that channel"));
	}
	return (true);
}

/**
 * @brief looks up TARG among members and if found, kicks him from this channel
 * 
 * @param src 
 * @param targ 
 * @param comment 
 * @return int 
 */
int		Channel::kick_op(const std::string& src, const std::string& targ, \
	const std::string& comment)
{
	for (std::map<int, Channel::Member>::iterator it = this->_members.begin(); \
		it != this->_members.end(); it++)
	{
		if ((*it).second.getName() == Libftpp::strToLower(targ))
		{
			(*it).second.getUser().disjoinChannel(this->_name);
			this->_members.erase((*it).first);
			if (!informMembers(src, "KICK " + this->_name + " " + targ, comment))
				return (false);
			return (true);
		}
	}
	return (_ignore);
}

/**
 * @brief Checks if all is right before operating the INVITE process
 * 
 * @param index 
 * @param src 
 * @param target 
 * @return int 
 */
int		Channel::invite(const size_t& index, const std::string& src, const std::string& target)
{
	if (!isMember(index))
	{
		Server::getUser(index)->insertOutMessage(\
			Server::numericMessage(HOSTNAME, ERR_NOTONCHANNEL, \
			src + " " + this->_name, "You're not on that channel"));
		return (_ignore);
	}
	if (isInviteOnly() && \
		!this->_members.at(Server::getSockfd(index)).isOperator())
	{
		Server::getUser(index)->insertOutMessage(\
			Server::numericMessage(HOSTNAME, ERR_CHANOPRIVSNEEDED, \
			src + " " + this->_name, "You're not channel operator"));
		return (_ignore);
	}
	if (isMember(target))
	{
		Server::getUser(index)->insertOutMessage(\
			Server::numericMessage(HOSTNAME, ERR_USERONCHANNEL, \
			src + " " + target + " " + this->_name, "is already on channel"));
		return (_ignore);
	}
	return (invite_op(index, src, target));
}

/**
 * @brief operates the INVITE process
 * 
 * @param index 
 * @param src 
 * @param target 
 * @return int 
 */
int		Channel::invite_op(const size_t& index, const std::string& src, const std::string& target)
{
	User*	_user;

	_user = NULL;
	_user = Server::getUser(target);
	if (!_user)
	{
		Server::getUser(index)->insertOutMessage(\
			Server::numericMessage(HOSTNAME, ERR_NOSUCHNICK, \
			src + " " + target, "No such nick"));
		return (_ignore);
	}
	Server::getUser(index)->insertOutMessage(\
		(std::string) ":" + HOSTNAME + " " + RPL_INVITING + " " + src \
		+ " " + target + " " + this->_name);
	_user->insertOutMessage(\
		Server::serverMessage(src, "INVITE " + target + " " + this->_name));
	this->_invited.insert(target);
	return (true);
}

int		Channel::userForceQuit(const int& sockfd, const std::string& username)
{
	informMembers(username, "PART " + this->_name);
	this->_members.erase(sockfd);
	return (true);
}
int		Channel::onChannelRemove(void)
{
// /*DEBUG*/std::cerr << "rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr" << '\n';
	for (std::map<int, Channel::Member>::iterator it = this->_members.begin(); \
		it != this->_members.end();  it++)
	{
		(*it).second.getUser().disjoinChannel(this->_name);
	}
	return (true);
}

const char*		Channel::UserNotFoundException::what(void) const throw()
{
	return ("user lookup failed");
}

/**
 * @brief Sends a message to all the members of the channel 
 * 		which amounts to send a message to the channel.
 * 		This function is like a template.
 * 
 * @param src 
 * @param cmd 
 * @param msg 
 * @return int 
 */
int		Channel::informMembers(const std::string& src, const std::string& cmd, \
	const std::string& msg)
{
	for (std::map<int, Channel::Member>::iterator it = this->_members.begin(); \
		it != this->_members.end(); it++)
	{
		// if ((*it).second.getName() == Libftpp::strToLower(src))
		// 	continue ;
		try
		{
			(*it).second.insertOutMessage(\
				Server::serverMessage(src, cmd, msg));
		}
		catch(const std::exception& e)
		{
/*DEBUG*/	std::cerr << "Error: in Channel::informMembers(): " << e.what() << '\n';
			return (false);
		}
	}
	return (true);
}
/**
 * @brief Sends a message to all the members of the channel 
 * 		which amounts to send a message to the channel.
 * 		This function is like a template.
 * 
 * @param src 
 * @param cmd 
 * @return int 
 */
int		Channel::informMembers(const std::string& src, const std::string& cmd)
{
	for (std::map<int, Channel::Member>::iterator it = this->_members.begin(); \
		it != this->_members.end(); it++)
	{
		// if ((*it).second.getName() == Libftpp::strToLower(src))
		// 	continue ;
		try
		{
			(*it).second.insertOutMessage(\
				Server::serverMessage(src, cmd));
		}
		catch(const std::exception& e)
		{
/*DEBUG*/	std::cerr << "Error: in Channel::informMembers(): " << e.what() << '\n';
			return (false);
		}
	}
	return (true);
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

/**
 * @brief  Informs members of the current topic of the channel.
 * 
 * @param _user source
 * @return int 
 */
int		Channel::rpl_topic(User& _user)
{
	if (topicIsSet())
	{
		_user.insertOutMessage( \
			Server::numericMessage(HOSTNAME, RPL_TOPIC, _user.getNickname() \
			+ " " + this->_name, getTopic()));
		_user.insertOutMessage( \
			Server::numericMessage(HOSTNAME, RPL_TOPICWHOTIME, _user.getNickname() \
			+ " " + this->_name + " " + this->_topic_author + " " + this->_topic_setat));
	}
	else
	{
		_user.insertOutMessage( \
			Server::numericMessage(HOSTNAME, RPL_NOTOPIC, _user.getNickname() \
			+ " " + this->_name, "No topic is set"));
	}
	return (true);
}

int		Channel::topic(const size_t& index, User& src, std::string& topic)
{
	if (!isMember(index))
	{
		src.insertOutMessage( \
			Server::numericMessage(HOSTNAME, ERR_NOTONCHANNEL, \
			src.getNickname() + " " + this->_name, "You're not on that channel"));
		return (_ignore);
	}
	Libftpp::trim(topic, " \t\r\n");
	if (topic.empty())
		return (rpl_topic(src));
	if (topicIsProtected() && !this->_members.at(Server::getSockfd(index)).isOperator())
	{
		src.insertOutMessage( \
			Server::numericMessage(HOSTNAME, ERR_CHANOPRIVSNEEDED, \
			src.getNickname() + " " + this->_name, "You're not channel operator"));
		return (_ignore);
	}
	return (changeTopic(src, topic));
}

int		Channel::changeTopic(User& src, std::string& topic)
{
	if (topic[0] == ':')
		topic.erase(topic.begin());
	if (topic.empty())
		clearTopic();
	else
		setTopic(src.getNickname(), topic);
	return (informMembers(src.getNickname(), "TOPIC " + this->_name, topic));
}
