/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelJoinCommand.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 18:18:18 by jm                #+#    #+#             */
/*   Updated: 2024/05/10 18:19:12 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

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
