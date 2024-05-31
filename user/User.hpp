/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfaye <jfaye@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 20:26:29 by TheTerror         #+#    #+#             */
/*   Updated: 2024/05/30 15:45:53 by jfaye            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include <poll.h>
# include <sys/socket.h>
# include <iostream>
# include <string>
# include <sstream>
# include <map>
# include <set>
# include "./../server/Server.hpp"
# include "./../libftpp/Libftpp.hpp"
# include "./../channel/Channel.hpp"
# include "./../Include.h"

class User
{
	public:

		User(int sockfd);
		~User(void);

		int					routine(const size_t& index, std::string buff);

		const std::string&	getNickname(void) const;
		const std::string&	getOutMsg(void) const;
		bool				isRegistered(void) const;
		bool				isoperator(void) const;
		bool				isRegUser(void) const;
		
		void				setUMode(const std::string& newmode);
		void				insertOutMessage(const std::string& msg);

		int					disjoinChannel(const std::string& chann_name);
		void				newJoinedChannel(const std::string& chann_name);
		bool				canJoinAChannel(const std::string& chann_name);
		int					onUserRemove(void);

	private:

		int							_sockfd;
		std::string					_cap;
		std::string					_password;
		std::string					_nickname;
		std::string					_oldnickname;
		std::string					_username;
		std::string					_hostname;
		std::string					_servername;
		std::string					_realname;
		std::string					_command;
		std::string					_parameters;
		bool						_registered;
		std::string					_inmsg;
		std::string					_outmsg;
		std::string					_umode;
		std::set<std::string>		_joinedchannels;
		std::map<std::string, int (User:: *)(const size_t& index)>	_map;

		int		parse(const size_t& index);
		int		capCommand(const size_t& index);
		int		passCommand(const size_t& index);
		int		nickCommand(const size_t& index);
		int		userCommand(const size_t& index);
		int		quitCommand(const size_t& index);
		int		lusersCommand(const size_t& index);
		int		pingCommand(const size_t& index);
		int		registrationComplete(void);
		int		transmitMessage(const size_t& index);
		int		joinCommand(const size_t& index);
		int		privmsgCommand(const size_t& index);
		int		kickCommand(const size_t& index);
		int		inviteCommand(const size_t& index);
		int		modeCommand(const size_t& index);
		int		topicCommand(const size_t& index);
		int		lusers_rpl_numerics(void);
		int		rpl_isupport_numerics(void);
		int		motd_rpl_numerics(void);

		int		leaveAllChannels(void);
		int		checkJoinTokens(std::string& channels_list, std::string& keys_list);
		int		tryJoin(const size_t& index, std::string& chan_name, std::string& key);
		int		numberOfTokens(std::string tokens);
		int		identifyTarget(const size_t& index, std::string& targets, const std::string& msg);
		int		tryTargetAsChannel(const size_t& index, std::string& targ, const std::string& msg, bool only_to_opers);
		int		tryTargetAsUser(std::string& targ, const std::string& msg);
		int		privmsgTowardsUser(User* user_targeted, std::string msg);
		int		tryKick(const size_t& index, const std::string& chann, std::string& targets, \
			std::string comment);
		int		tryInvite(const size_t& index, const std::string& target, std::string& chann);
		int		channelMode(const size_t& index, std::string& target, std::string& mode);
		int		userMode(std::string& target, std::string& mode);
		int		setAUserMode(std::string& rp_mode, std::string& mode, size_t& i);
		int		removeAUserMode(std::string& rp_mode,  std::string& mode, size_t& i);

};

#endif