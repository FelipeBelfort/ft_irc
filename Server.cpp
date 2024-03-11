#include "Server.hpp"

#define BUFF_SIZE 42

Server::Server()
{
}

Server::~Server()
{

}

void	Server::launchServer(const std::string &port, const std::string &password)
{
	float		p = atof(port.c_str());
	int			server_socket;
	sockaddr_in	server_addr;
	char		buff[BUFF_SIZE];
	int			read_ret = 1;
	int	client_socket;

	if (port.empty() || port.find_first_not_of("0123456789") != std::string::npos || p < MIN_PORT || p > MAX_PORT)
		return;
	this->_port = p;
	this->_password = password;

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1)
		return;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(this->_port);
	server_addr.sin_addr.s_addr = INADDR_ANY; 

	if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)))
		return;
	if (listen(server_socket, 5))
		return;
	
	struct pollfd	sockets[1];
	sockets[0].fd = server_socket;
	sockets[0].events = POLLIN;

	while (true)
	{
		int ret = poll(sockets, 1, -1);
		if (ret < 0)
			break; // error in poll()
		else if (!ret)
			continue; // timeout
		else
		{
			if (sockets[0].revents & POLLIN)
				client_socket = accept(server_socket, NULL, NULL);

			do {
				read_ret = recv(client_socket, buff, BUFF_SIZE, 0);
				buff[read_ret] = 0;
				std::cout << " " << client_socket << " -> " << buff;
			}	while (read_ret > 0);
		}
	}
	

	// struct sockaddr_storage	client_addr;
	// socklen_t	addr_size = sizeof(client_addr);


	
}