#include "Server.hpp"

void    Server::botSayHello(User& user)
{
    std::string msg = "Hello World!";
    user.insertOutMessage(\
					Server::serverMessage(BOT_NAME, "PRIVMSG " + user.getNickname(), msg));
}

bool     Server::botInteraction(User& user, const std::string& msg)
{
    // std::cout << "test |" << msg << "|" << std::endl;
    if (msg == ":hello")
        user.insertOutMessage(\
					Server::serverMessage(BOT_NAME, "PRIVMSG " + user.getNickname(), "blabla"));
    else
        user.insertOutMessage(\
					Server::serverMessage(BOT_NAME, "PRIVMSG " + user.getNickname(), "I don't understand"));
    return (true);
}

// void    Server::launchBot(void)
// {
//     static std::
//     for (std::map<int, User>::iterator it = _users.begin(); it != _users.end(); it++)
// 	{

// 		(*it).second.insertOutMessage(\
// 						Server::serverMessage(BOT_NAME, "PRIVMSG " + (*it).second.getNickname(), "Hello World"));
// 	}
// }