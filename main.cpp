#include "Server.hpp"

int main(int argc, char **argv)
{
	Server	server;

	if (argc != 3)
	{
		std::cout << "Error: Expected ./ircserv <port> <password>" << std::endl;
		return EXIT_FAILURE;
	}
	try
	{
		server.launchServer(argv[1], argv[2]);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}