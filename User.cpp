#include "User.hpp"

User::User(int fd) //: _fd(fd), _is_logged(false)
{
	(void) fd;
	this->_msg.clear();
}

User::~User(void)
{

}

void	User::getMessage(std::string &msg)
{
	this->_msg += msg;
}

std::string	User::sendMessage(void)
{
	std::string	tmp = this->_msg;
	this->_msg.clear();
	return tmp;
}

const std::string	User::getNick(void)
{
	return this->_nickname;
}