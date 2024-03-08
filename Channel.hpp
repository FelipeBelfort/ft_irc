#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <vector>
# include <User.hpp>

class Channel
{
private:
	bool						_invite_only;
	bool						_topic_is_locked;
	std::string					_topic;
	std::string					_key;
	std::vector<const User &>	_users;
	std::vector<int>			_operators;
	bool						_limit_is_set;
	size_t						_limit;
public:
	Channel();
	~Channel();

	void	joinChannel(const User &user);
	void	quitChannel(const User &user);

	const bool			getInviteOnly(void) const;
	const bool			getTopicIsLocked(void) const;
	const std::string	getTopic(void) const;
	const std::string	getKey(void) const;
	const bool			getLimitIsSet(void) const;
	const size_t		getLimit(void) const;

	void				setTopic();
};

#endif