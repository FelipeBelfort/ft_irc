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
# include <list>
# include "User.hpp"
// # include "Channel.hpp"

# define MIN_PORT 1023
# define MAX_PORT 65535
# define BUFF_SIZE 42
# define MAX_CONNECTIONS 20

class User;

class Server
{
private:
	static uint16_t				_port;
	static std::string				_password;
	static std::vector<pollfd>		_sockets;
	// std::list<pollfd>		_sockets;
	static std::map<int, User>	_clients;
	// std::map<std::string, Channel*> _channels;
	static bool	initServer(const std::string &port, const std::string &password);
	static void	createUser(void);
	static void	closeClient(int i);
	static bool isValidNick(const std::string &nick);
	static bool isUniqueNick(const std::string &nick);

public:
	static bool	is_connected;
	Server();
	~Server();

	static void	launchServer(const std::string &port, const std::string &password);
};

void	exitServer(int sign);

#endif
