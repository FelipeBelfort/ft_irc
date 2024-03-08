#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <sys/socket.h>
# include <sys/select.h>

class Server
{
private:
	bool	_is_connected;
public:
	Server();
	~Server();

	void	launchServer(std::string port, std::string password);
};



#endif