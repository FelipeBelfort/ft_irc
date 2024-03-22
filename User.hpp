#ifndef USER_HPP
# define USER_HPP

# include <iostream>

class User
{
private:
	std::string	_nickname;
	std::string	_username;
	int			_fd;
public:
	User(std::string nickname, std::string username);
	~User();

	void	joinChannel(std::string channel);
	void	sendMessage(std::string message, std::string channel);
};

#endif