/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommands.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 18:52:58 by jm                #+#    #+#             */
/*   Updated: 2024/05/10 15:41:10 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

/**
 * @brief The server acknowledges QUIT command by replying with an 
 * 		ERROR message and closing the connection to the client 
 * 		by returning '_fatal'
 * 
 * @param index 
 * @return _fatal 
 */
int		User::quitCommand(const size_t& index)
{
	(void)	index;
	this->_outmsg += Server::ErrorMessage(" ");
/*DEBUG*/std::cout << this->_nickname << " quitted!" << std::endl;
	return (_fatal);
}

int		User::lusersCommand(const size_t& index)
{
	(void)	index;
	if (!this->_registered)
		return (_ignore);
	this->_outmsg += Server::numericMessage(HOSTNAME, RPL_LUSERCLIENT, \
		this->_nickname, "There are <u> users and <i> invisible on <s> servers");
	return (true);
}

/**
 * @brief PING command processor.
 * 		Parses parameters then reply with PONG message
 * 
 * @param index 
 * @return int 
 */
int		User::pingCommand(const size_t& index)
{
	(void)	index;
	if (!this->_registered)
		return (_ignore);
	Libftpp::trim(this->_parameters, " \t");
	if (this->_parameters.empty())
		this->_outmsg += Server::numericMessage(HOSTNAME, ERR_NEEDMOREPARAMS, \
			this->_nickname + " PING", "Not enough parameters");
	this->_outmsg += (std::string) ":" + HOSTNAME + " PONG " + \
		this->_parameters + "\r\n";
	return (true);
}

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

// Command: PRIVMSG
// Parameters: <target>{,<target>} <text to be sent>
/**
 * @brief Checks if the parameters pattern is rightful and
 * 		then launches the identification of the targets (user or channel)
 * 		with identifyTarget();
 * 
 * @param index 
 * @return int 
 */
int		User::privmsgCommand(const size_t& index)
{
	int				fdbk;
	std::string		targets;
	std::string		msg;

	fdbk = true;
	if (!this->_registered)
	{
		this->_outmsg += Server::numericMessage(HOSTNAME, ERR_NOTREGISTERED, \
			this->_nickname, "you have not registered");
		return (_ignore);
	}
	targets = Libftpp::extractStr(this->_parameters, " \t\r\n", false);
	msg = this->_parameters;
	Libftpp::trim(targets, " \t\r\n");
	Libftpp::trim(msg, " \t\r\n");
	if (targets.empty())
	{
		this->_outmsg += Server::numericMessage(HOSTNAME, ERR_NORECIPIENT, \
			this->_nickname, "No recipient given PRIVMSG");
		return (_ignore);
	}
	if (msg.empty())
	{
		this->_outmsg += Server::numericMessage(HOSTNAME, ERR_NOTEXTTOSEND, \
			this->_nickname, "No text to send");
		return (_ignore);
	}
	fdbk = identifyTarget(index, targets, msg);
	return (fdbk);
}

/**
 * @brief Loops on TARGETS and for each target identifies 
 * 		the type (user or channel) to launch the respective part
 * 
 * @param index 
 * @param targets 
 * @param msg 
 * @return int 
 */
int		User::identifyTarget(const size_t& index, std::string& targets, const std::string& msg)
{
	int			fdbk;
	bool		only_to_opers;

	only_to_opers = false;
	for (std::string targ; !targets.empty(); )
	{
		targ.assign(Libftpp::extractStr(targets, ",\r\n", false));
		if (targ[0] == '@')
		{
			only_to_opers = true;
			Libftpp::trimStart(targ, "@", 1);
		}
		if (!targ.empty() && (targ[0] == '&' || targ[0] == '#'))
		{
			fdbk = tryTargetAsChannel(index, targ, msg, only_to_opers);
			if (!fdbk)
				return (false);
			if (fdbk == true)
				continue ;
		}
		fdbk = tryTargetAsUser(targ, msg);
		if (!fdbk)
			return (false);
		if (fdbk == true)
			continue ;
	}
	return (true);
}

/**
 * @brief Searches firstly the targeted channel, if found it launches 
 * 		its privmsg service (privmsg())
 * 
 * @param index 
 * @param targ 
 * @param msg 
 * @param only_to_opers to target the channel operators only
 * @return int 
 */
int		User::tryTargetAsChannel(const size_t& index, std::string& targ, \
	const std::string& msg, bool only_to_opers)
{
	for (std::map<std::string, Channel>::iterator it = Server::channels.begin(); \
		it != Server::channels.end(); it++)
	{
		if (Libftpp::strToLower((*it).second.getName()) == Libftpp::strToLower(targ))
		{
			return ((*it).second.privmsg(index, this->_nickname, targ, msg, only_to_opers));
		}
	}
	this->_outmsg += Server::numericMessage(HOSTNAME, ERR_NOSUCHCHANNEL, \
		this->_nickname + " " + targ, "No such channel");
	return (_ignore);
}

/**
 * @brief Searches firstly the targeted user, if found it launches 
 * 		its privmsg service (privmsgTowardsUser())
 * 
 * @param targ 
 * @param msg 
 * @return int 
 */
int		User::tryTargetAsUser(std::string& targ, const std::string& msg)
{
	User*	_user;

	for (size_t i = 1; i < Server::getSocketsSize(); i++)
	{
		_user = NULL;
		_user = Server::getUser(i);
		if (!_user)
			return (false);
		if (Libftpp::strToLower(_user->getNickname()) == Libftpp::strToLower(targ))
		{
			if (!privmsgTowardsUser(_user, msg))
				return (false);
			return (true);
		}
	}
	this->_outmsg += Server::numericMessage(HOSTNAME, ERR_NOSUCHNICK, \
		this->_nickname + " " + targ, "No such nick");
	return (_ignore);
}

/**
 * @brief sends the message to the targeted user
 * 
 * @param user_targeted 
 * @param msg 
 * @return int 
 */
int		User::privmsgTowardsUser(User* user_targeted, std::string msg)
{
	if (msg[0] == ':')
	{
		Libftpp::trimStart(msg, ":", 1);
		user_targeted->insertOutMessage(\
			Server::serverMessage(this->_nickname, "PRIVMSG " + \
				user_targeted->getNickname(), msg));
		return (true);
	}
	else
	{
		msg = Libftpp::extractStr(msg, " :\t\r\n", false);
		user_targeted->insertOutMessage(\
			Server::serverMessage(this->_nickname, "PRIVMSG " + \
				user_targeted->getNickname(), msg));
		return (true);
	}
	return (true);
}

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
