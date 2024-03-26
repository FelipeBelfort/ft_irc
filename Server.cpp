#include "Server.hpp"

#define BUFF_SIZE 42
#define MAX_CONNECTIONS 5

bool	Server::is_connected = true;

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
		return; // TODO add error message and error return
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(this->_port);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)))
		return;
	if (listen(server_socket, MAX_CONNECTIONS))
		return;

	pollfd	socket;
	socket.fd = server_socket;
	socket.events = POLLIN;

	std::vector<pollfd>	sockets;

	sockets.push_back(socket);

	// std::string	response = "001: Welcome! \r\n";


	while (is_connected)
	{
		signal(SIGINT, &exitServer);

		sockets[0].revents = 0;
		int ret = poll(&sockets[0], sockets.size(), 10);
		if (ret < 0)
			break; // error in poll()
		else if (!ret)
			continue; // timeout
		if (sockets[0].revents & POLLIN)	// TODO => create a new client
			client_socket = accept(server_socket, NULL, NULL);
		for (size_t i = 1; i < sockets.size(); i++)
		{

			do {
				read_ret = recv(client_socket, buff, BUFF_SIZE, 0);
				buff[read_ret] = 0;
				std::cout << " sockets => " << sockets[i].fd << " client => " << client_socket << " -> " << buff;
			}	while (read_ret > 0 && is_connected);


			sockets[i].revents = 0;
		}
		// else
		// {

		// 	std::cout << "\nbla\n";
		// 	// send(client_socket, response.c_str(), response.size(), 0);
		// }
	}

	// struct sockaddr_storage	client_addr;
	// socklen_t	addr_size = sizeof(client_addr);



	std::cout << "\b\b\nExiting Server..." << std::endl;
}

void	Server::createUser(void)
{
	pollfd	user_socket;
	user_socket.fd = accept(server_socket, NULL, NULL);
	user_socket.events = POLLIN | POLLOUT;
}

void	exitServer(int sign)
{
	if (sign == SIGINT)
		Server::is_connected = false;
}
