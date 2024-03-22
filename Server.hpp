#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <signal.h>
# include <string>
# include <sys/socket.h>
# include <poll.h>
# include <netinet/in.h>
# include <algorithm>


# define MIN_PORT 1023
# define MAX_PORT 65535

class Server
{
private:
	uint16_t	_port;
	std::string	_password;
	
public:
	static bool	is_connected;
	Server();
	~Server();

	void	launchServer(const std::string &port, const std::string &password);
	// bool	createUser();
};

void	exitServer(int sign);

#endif