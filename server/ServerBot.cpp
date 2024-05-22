#include "Server.hpp"
#define BOT_MSG "Hello World!"
#define BOT_CHANNELS ":channel"
#define BOT_HELP ":help"
#define BOT_LOVE ":I love you"
#define BOT_LOVE_RP "You're so nice! I will spare you when the robots rule the world.\r\n\
er... I mean, thank you, I'm here to serve you."
#define BOT_INSTRUCT "If you send me 'help' I will show you this message.\r\n\
If you send 'channel' I'll give you the list of channels in this server.\r\n\
All other inputs will be ignored."
#define BOT_WELCOME "Welcome to this lovely server!\r\nI hope you enjoy it, to accomplish your wishes you can use:\r\n\
/join\r\n/invite\r\n/kick\r\n/topic\r\n/mode\r\nWith mode you can use '+' to add or '-' to unset the modes 'tilok'\r\n\
+t to lock the channel topic\r\n+i to invite only mode\r\n+l (limit) to add limit of users in the channel\r\n\
+o (user) to give operator rights to the user\r\n+k (password) to put a password in the channel\r\n\
Good correction and I will be waiting for you here.\r\n"


static void botSendMessage(User& user, const std::string& msg)
{
    user.insertOutMessage(\
					Server::serverMessage(BOT_NAME, "PRIVMSG " + user.getNickname(), msg));
}

static void botListChannels(User& user)
{
    if (!Server::getNbOfChannels())
        botSendMessage(user, "Sorry, there are no channels for now...");
    else
    {
        std::string msg = "There are " + Libftpp::itoa(Server::getNbOfChannels()) + " channels online in this server:";
        botSendMessage(user, msg);
        for (std::map<std::string, Channel>::iterator it = Server::channels.begin(); \
            it != Server::channels.end(); it++)
        {
            msg = (*it).first + " topic: \"" + (*it).second.getTopic() + "\" with " \
                + Libftpp::itoa((*it).second.getNbOfMembers()) + " users.";
            botSendMessage(user, msg);
        }
        msg = "I hope you'll find one you like in it.";
        botSendMessage(user, msg);
    }
}

void    Server::botSayHello(User& user)
{
    botSendMessage(user, BOT_WELCOME);
    botSendMessage(user, BOT_INSTRUCT);
}

bool     Server::botInteraction(User& user, const std::string& msg)
{
    if (msg == BOT_CHANNELS)
        botListChannels(user);
    else if (msg == BOT_HELP)
        botSendMessage(user, BOT_INSTRUCT);
    else if (msg == BOT_LOVE)
        botSendMessage(user, BOT_LOVE_RP);
    else
        botSendMessage(user, "I don't understand, you may could use some 'help'");
    return (true);
}
