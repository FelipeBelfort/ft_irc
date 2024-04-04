#ifndef USER_HPP
# define USER_HPP

# include <iostream>

class User
{
private:
	// std::string	_nickname;
	// std::string	_username;
	// int			_fd;
	// bool		_is_logged;
	std::string	_msg;

public:
	User(int fd);
	~User(void);

	void	getMessage(std::string &msg);
	// void	joinChannel(std::string channel);
	std::string	sendMessage(); // std::string msg, std::string channel);
};

#endif