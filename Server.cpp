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

/**
 * @brief 
 * It initiates the server. If the PORT is not a valid port or if socket() fails it returns false,
 *  if bind() or listen() fails it closes the fd and return false.
 * In case of success it pushs the server socket to the list of sockets and returns true. 
 * The password is setted by default
 * 
 * @param port expected a int between MIN_PORT and MAX_PORT
 * @param password 
 * @return false
 * @return true 
 */
bool	Server::initServer(const std::string &port, const std::string &password)
{
	float		p;
	pollfd		server_socket;
	sockaddr_in	server_addr;


	if (port.empty() || port.find_first_not_of("0123456789") != std::string::npos) 
		return false;
	p = atof(port.c_str());
	if (p < MIN_PORT || p > MAX_PORT)
		return false;

	this->_port = p;
	this->_password = password;

	server_socket.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket.fd == -1)
		return false; 
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(this->_port);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(server_socket.fd, (struct sockaddr *)&server_addr, sizeof(server_addr)))
		return (close(server_socket.fd), false);
	if (listen(server_socket.fd, MAX_CONNECTIONS))
		return (close(server_socket.fd), false);
	server_socket.events = POLLIN;

	this->_sockets.push_back(server_socket);
	return true;
}

/**
 * @brief 
 * It calls the initServer() and if it returns true 
 *  then it calls the signal() to catch 'ctrl+c' and launchs the loop with pool() to listen 
 * the sockets
 *  
 * TODO => error management and the actions inside the for loop
 * 
 * @param port  to be used in initServer() 
 * @param password  to be used in initServer()
 */
void	Server::launchServer(const std::string &port, const std::string &password)
{
	char		buff[BUFF_SIZE];
	int			read_ret;

	if (!this->initServer(port, password))
		return; // TODO error msg


	std::string	test;
	signal(SIGINT, &exitServer);
	while (is_connected)
	{
		test.clear();

		this->_sockets[0].revents = 0;
		int ret = poll(&this->_sockets[0], this->_sockets.size(), 10);
		if (ret < 0)
			break; // error in poll()
		else if (!ret)
			continue; // timeout
		if (this->_sockets[0].revents & POLLIN)	// TODO => create a new client
			this->createUser();
			// client_socket = accept(this->_sockets[0].fd, NULL, NULL);
		for (size_t i = 1; i < this->_sockets.size(); i++)
		{
			if (this->_sockets[i].revents & POLLIN)
			{
				// do {
					memset(buff, 0, BUFF_SIZE);
					read_ret = recv(this->_sockets[i].fd, buff, BUFF_SIZE, MSG_DONTWAIT);
					buff[read_ret] = 0;
					// std::cout << " client => " << this->_sockets[i].fd << " read_ret => " << read_ret << " -> " << buff;
					test = " client => ";
					test += (char)this->_sockets[i].fd + '0';
					test += " read_ret => ";
					test += (char)read_ret + '0';
					test += " -> ";
					test += buff;
				// }	while (read_ret > 0 && is_connected);
				if (!read_ret)
				{
					std::cout << "client => " << this->_sockets[i].fd << " closed" << std::endl;
					close(this->_sockets[i].fd);
					this->_sockets.erase(this->_sockets.begin() + i);
				}

			}
			if (this->_sockets[i].revents & POLLOUT)
				if (!test.empty())
					send(this->_sockets[i].fd, test.c_str(), test.size(), MSG_DONTWAIT);

			if (this->_sockets[i].revents & POLLERR)
			{
				std::cout << "client => " << this->_sockets[i].fd << " POLLERR" << std::endl;
				close(this->_sockets[i].fd);
				this->_sockets.erase(this->_sockets.begin() + i);
			}
			if (this->_sockets[i].revents & POLLHUP)
			{
				std::cout << "client => " << this->_sockets[i].fd << " POLLHUP" << std::endl;
				close(this->_sockets[i].fd);
				this->_sockets.erase(this->_sockets.begin() + i);
			}	
				// std::cout << " client => " << this->_sockets[i].fd << " ready to POLLOUT " ;



			this->_sockets[i].revents = 0;
		}
	}




	std::cout << "\nExiting Server..." << std::endl;
}

/**
 * @brief 
 * Fonction to create a user socket and push it in the list
 * TODO => error management and creation of a User object
 * 
 */
void	Server::createUser(void)
{
	pollfd	user_socket;

	user_socket.fd = accept(this->_sockets[0].fd, NULL, NULL);
	if (user_socket.fd == -1)
		return; // TODO error msg
	// fcntl(user_socket.fd, F_SETFL, O_NONBLOCK);
	user_socket.events = POLLIN | POLLOUT;
	this->_sockets.push_back(user_socket);
}

/**
 * @brief 
 * Handle the SIGINT and changes the is_connected state to exit the server
 * 
 * @param sign 
 */
void	exitServer(int sign)
{
	if (sign == SIGINT)
		Server::is_connected = false;
}
