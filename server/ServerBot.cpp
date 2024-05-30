#include "Server.hpp"
#define BOT_MSG "Hello World!"
#define BOT_CHANNELS ":channel"
#define BOT_HELP ":help"
#define BOT_LOVE ":i love you"
#define BOT_LOVE_RP "You're so nice! I will spare you when the robots rule the world.\
er... I mean, thank you, I'm here to serve you."
#define BOT_INSTRUCT "If you send me 'help' I will show you this message.\
If you send 'channel' I'll give you the list of channels in this server.\
All other inputs will be ignored."
#define BOT_WELCOME "Welcome to this lovely server! I hope you enjoy it, to accomplish your wishes you can use:"
#define JOIN_CMD "/join"
#define INVITE_CMD "/invite"
#define KICK_CMD "/kick"
#define TOPIC_CMD "/topic"
#define MODE_CMD "/mode"
#define MODE_CMD_INFO "With mode you can use '+' to add or '-' to unset the modes 'tilok'"
#define MODE_CMD_T "+t to lock the channel topic"
#define MODE_CMD_I "+i to invite only mode"
#define MODE_CMD_L "+l (limit) to add limit of users in the channel"
#define MODE_CMD_O "+o (user) to give operator rights to the user"
#define MODE_CMD_K "+k (password) to put a password in the channel"
#define BOT_WELCOME_END "Good correction and I will be waiting for you here."


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
    botSendMessage(user, JOIN_CMD);
    botSendMessage(user, INVITE_CMD);
    botSendMessage(user, KICK_CMD);
    botSendMessage(user, TOPIC_CMD);
    botSendMessage(user, MODE_CMD);
    botSendMessage(user, MODE_CMD_INFO);
    botSendMessage(user, MODE_CMD_T);
    botSendMessage(user, MODE_CMD_I);
    botSendMessage(user, MODE_CMD_L);
    botSendMessage(user, MODE_CMD_O);
    botSendMessage(user, MODE_CMD_K);
    botSendMessage(user, BOT_WELCOME_END);
    botSendMessage(user, BOT_INSTRUCT);
}

bool     Server::botInteraction(User& user, const std::string& msg)
{
    if (Libftpp::strToLower(msg) == BOT_CHANNELS)
        botListChannels(user);
    else if (Libftpp::strToLower(msg) == BOT_HELP)
        botSendMessage(user, BOT_INSTRUCT);
    else if (Libftpp::strToLower(msg) == BOT_LOVE)
        botSendMessage(user, BOT_LOVE_RP);
    else
        botSendMessage(user, "I don't understand, you may could use some 'help'");
    return (true);
}
