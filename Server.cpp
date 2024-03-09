#include "Server.hpp"

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
	
}