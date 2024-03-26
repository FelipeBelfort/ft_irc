#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <signal.h>
# include <unistd.h>
# include <fcntl.h>
# include <string>
# include <cstring>
# include <sys/socket.h>
# include <poll.h>
# include <netinet/in.h>
# include <algorithm>
# include <vector>
# include <map>
// # include "User.hpp"
// # include "Channel.hpp"

# define MIN_PORT 1023
# define MAX_PORT 65535

class Server
{
private:
	uint16_t				_port;
	std::string				_password;
	std::vector<pollfd>		_sockets;
	// std::map<int, User*>	_clients;
	// std::map<std::string, Channel*> _channels;

public:
	static bool	is_connected;
	Server();
	~Server();

	void	launchServer(const std::string &port, const std::string &password);
	bool	initServer(const std::string &port, const std::string &password);
	void	createUser(void);
};

void	exitServer(int sign);

#endif
