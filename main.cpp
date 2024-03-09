#include "Server.hpp"

int main(int argc, char **argv)
{
	Server	server;

	if (argc != 3)
	{
		std::cout << "Error: Expected ./ircserv <port> <password>" << std::endl;
		return 1;
	}
	try
	{
		server.launchServer(argv[1], argv[2]);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}

	return 0;
}