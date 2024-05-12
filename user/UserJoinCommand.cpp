/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserJoinCommand.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 18:02:02 by jm                #+#    #+#             */
/*   Updated: 2024/05/12 01:31:04 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

// Command: JOIN
// Parameters: <channel>{,<channel>} [<key>{,<key>}]
// Alt Params: 0
/**
 * @brief Extracts a channel name or a list of channel names
 * 		and eventually a list of respective access keys from 
 * 		parameters ('_parameters').
 * 		Then if all is right launches tryJoin() on each channel name.
 * 
 * @param index 
 * @return int 
 */
int		User::joinCommand(const size_t& index) //TODO recheck carefully the join command with the different cases
{
	int				fdbk;
	std::string		channels_list;
	std::string		keys_list;

	fdbk = true;
	if (!this->_registered)
	{
		this->_outmsg += Server::numericMessage(HOSTNAME, ERR_NOTREGISTERED, \
			this->_nickname, "you have not registered");
		return (_ignore);
	}
	Libftpp::trim(this->_parameters, " \t\r\n");
	if (this->_parameters == "0")
		return (leaveAllChannels());
	channels_list = Libftpp::extractStr(this->_parameters, " \t\r\n", false);
	keys_list = this->_parameters;
	Libftpp::trim(channels_list, " \t\r\n");
	Libftpp::trim(keys_list, " \t\r\n");
	fdbk = checkJoinTokens(channels_list, keys_list);
	if (fdbk != true)
		return (fdbk);
	for (std::string chan_name, key; !channels_list.empty(); \
		chan_name.clear(), key.clear())
	{
		chan_name.assign(Libftpp::extractStr(channels_list, ",\r\n", false));
		key.assign(Libftpp::extractStr(keys_list, ",\r\n", false));
		if (chan_name[0] != '#' && chan_name[0] != '&')
		{
			this->_outmsg += Server::numericMessage(HOSTNAME, ERR_BADCHANMASK, \
				this->_nickname, "Bad Channel Mask");
			return (_ignore);
		}
		fdbk = tryJoin(index, chan_name, key);
		if (fdbk != true)
			return (fdbk);
	}
	return (true);
}

int		User::leaveAllChannels(void)
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
/*DEBUG*/	std::cerr << "Fatal: leaveAllChannels() failed: " << e.what() << '\n';
		}
	}
	return (true);
}

/**
 * @brief makes sure that there is at least one channel on 
 * 		the list and that the 'keys_list' content is 
 * 		well formatted for a loop on each channel name and 
 * 		its eventual matched keyword
 * 
 * @param channels_list 
 * @param keys_list 
 * @return int 
 */
int		User::checkJoinTokens(std::string& channels_list, std::string& keys_list)
{
	size_t	nb_channs;
	size_t	nb_keys;

	if (channels_list.empty())
	{
		this->_outmsg += Server::numericMessage(HOSTNAME, ERR_NEEDMOREPARAMS, \
			this->_nickname + " JOIN", "Not enough parameters");
		return (_ignore);
	}
	nb_channs = numberOfTokens(channels_list);
	nb_keys = numberOfTokens(keys_list);
	if (!nb_keys)
	{
		keys_list.clear();	
		for (size_t i = 1; i < nb_channs; i++)
			keys_list.append(",");
	}
	else if (nb_keys < nb_channs)
	{
		this->_outmsg += Server::numericMessage(HOSTNAME, ERR_NEEDMOREPARAMS, \
			this->_nickname + " JOIN", "Not enough parameters, asymetric tokens list");
		return (_ignore);
	}
	return (true);
}

int		User::numberOfTokens(std::string tokens)
{
	int		nb;

	for (nb = 0; !tokens.empty(); nb++)
		Libftpp::extractStr(tokens, ",", false);
	return (nb);
}

/**
 * @brief Checks firstly if the channel name exists yet.
 * 		If it exists it launches joinChannel() to try 
 * 		to join this user to that channel.
 * 		Else, it creates first the channel and then join 
 * 		the user to that one.
 * 
 * @param index index of this user's socket in sockets container
 * @param chan_name name of the channel to join
 * @param key eventual keyword
 * @return int 
 */
int		User::tryJoin(const size_t& index, std::string& chan_name, std::string& key)
{
	int		fdbk;

	fdbk = true;
	try
	{
		fdbk = Server::channels.at(Libftpp::strToLower(chan_name)\
			).joinChannel(index, key);
	}
	catch(const std::exception& e)
	{
		std::cerr << "tryJoin() exception: " << e.what() << '\n';
		Server::channels.insert(std::pair<std::string, Channel>(\
			Libftpp::strToLower(chan_name), Channel(chan_name, key)));
		try
		{
			fdbk = Server::channels.at(Libftpp::strToLower(chan_name) \
				).joinChannel(index, key);
			if (fdbk == _error)
				return (Server::removeChannel(chan_name), _ignore);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
			this->_outmsg += Server::numericMessage(HOSTNAME, \
				ERR_NOSUCHCHANNEL, this->_nickname + " " + chan_name, \
				"No such channel");
			return (Server::removeChannel(chan_name), _ignore);
		}
	}
	return (fdbk);
}
