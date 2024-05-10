/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 21:59:10 by jm                #+#    #+#             */
/*   Updated: 2024/05/10 17:09:26 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <ctime>
# include <iostream>
# include <vector>
# include <set>
# include <exception>
# include "./../user/User.hpp"

class User;

class Channel
{
	public:

		Channel(const std::string &name, const std::string& key);
		~Channel();


		int	joinChannel(const size_t& id, const std::string& key);
		// void	quitChannel(const User &user);

		bool				isMember(const size_t& index) const;
		bool				isMember(const int& sockfd) const;
		bool				isMember(const std::string& nick);
		const std::string&	getName(void) const;
		bool				isInviteOnly(void) const;
		bool				requireKey(void) const;
		bool				topicIsProtected(void) const;
		bool				isLimited(void) const;
		bool				topicIsSet(void) const;
		bool				userIsInvited(const std::string& _username) const;
		const std::string	getTopic(void) const;
		const std::string	getKey(void) const;
		const size_t&		getLimit(void) const;
		const std::string	getCreationTime(void) const;

		void				setKey(const std::string& key);
		void				setTopicProtection(bool b);
		void				setInviteOnly(bool b);
		void				setLimit(int limit);
		void				setTopic(const std::string& author, \
			const std::string& topic);
		void				unsetKey(void);
		void				unsetLimit(void);
		void				unsetInviteOnly(void);
		void				unsetTopicProtection(void);
		void				clearTopic(void);
		void				rmFromInvitedList(const std::string& username);

		int					privmsg(const size_t& index, const std::string& src, const std::string& targ, \
			std::string msg, bool only_to_opers);
		int					kick(const size_t& index, const std::string& src, std::string targets, \
			const std::string& comment);
		int					invite(const size_t& index, const std::string& src, const std::string& target);
		int					mode(const size_t& index, User& src, std::string& mode);
		int					topic(const size_t& index, User& src, std::string& topic);

		int					rpl_channelModeIs(User& _user);
		int					rpl_creationTime(User& _user);
		int					rpl_topic(User& _user);
		int					userForceQuit(const int& sockfd, const std::string& username);
		int					onChannelRemove(void);

		class UserNotFoundException : public std::exception
		{
			public :
				const char*	what(void) const throw();
		};
		
	private:

		Channel();

		class Member
		{
			public:

				Member(const size_t& index);
				~Member();

				const size_t&		getIndex(void) const;
				const int&			getSock_fd(void) const;
				User&				getUser(void) const;
				const std::string&	getName(void) const;
				bool				isOperator(void) const;
			
				void				setIndex(const size_t& id);
				void				setOperator(bool b);
				void				insertOutMessage(const std::string& msg);


			private:

				Member();

				size_t			_index;
				bool			_operator;
		};

		std::string					_name;
		std::string					_key;
		bool						_key_required;
		bool						_invite_only;
		bool						_topic_is_locked;
		std::string					_topic;
		std::string					_topic_author;
		std::string					_topic_setat;
		// std::vector<int>			_operators;
		bool						_limit_is_set;
		size_t						_limit;
		bool						_isPionner_flag;
		std::string					_creationTimestamp;
		std::map<int, Member>		_members;
		std::set<std::string>		_invited;

		int		isNotMember(const int& sockfd);
		int		canJoinTheChannel(User* _user, \
			const std::string& username, const std::string& key);
		int		syncInsertion(const size_t& id, const Member& new_memb);
		int		acknowledgementSequence(User* _user, const std::string& username);
		int		informMembers(const std::string& src, const std::string& cmd, \
			const std::string& msg);
		int		informMembers(const std::string& src, const std::string& cmd);
		int		kick_op(const std::string& src, const std::string& targ, \
			const std::string& comment);
		int		invite_op(const size_t& index, const std::string& src, const std::string& target);
		int		applyModes(User& src, std::string& mode, std::string& args);
		int		setChannelModes(size_t& i, User& src, std::string& mode, std::string& args, std::string& rp_mode, std::string& rp_args);
		int		removeChannelModes(size_t& i, User& src, std::string& mode, std::string& args, std::string& rp_mode, std::string& rp_args);
		int		changeTopic(User& src, std::string& topic);
};

#endif
