/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Include.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 16:57:31 by jm                #+#    #+#             */
/*   Updated: 2024/05/07 19:44:18 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDE_H
# define INCLUDE_H

typedef enum    e_type
{
    _false, _true, _ignore, _fatal, _error
}               t_type;

# define SERVERNAME				"ircserver"
# define HOSTNAME				"localhost"

# define DEFAULT_LIMIT			4


//TODO review macros below by making them dynamic with data information holders
# define CASEMAPPING			"ascii"
# define CHANLIMITPREFIXES		"#&:"
# define CHANLIMIT				42
# define CHANNELLEN				24
# define CHANTYPES				"&#"
# define CHANMODES				"i,t,k,l"
# define PREFIX					"(o)@"
# define KICKLEN				242
# define MODES					5
# define NICKLEN				24
# define STATUSMSG				"@"
# define TOPICLEN				242
# define USERLEN				24

/**
 * @brief numeric replies Macros
 */

/** @brief   "<client> :Welcome to the <networkname> Network, <nick>[!<user>@<host>]" */
# define RPL_WELCOME			"001"

/** @brief   "<client> :Your host is <servername>, running version <version>" */
# define RPL_YOURHOST			"002"

/** @brief   "<client> :This server was created <datetime>" */
# define RPL_CREATED			"003"

/** @brief "<client> <servername> <version> <available user modes> <available channel modes> [<channel modes with a parameter>]" */
# define RPL_MYINFO				"004"

/** @brief   "<client> <1-13 tokens> :are supported by this server" */
# define RPL_ISUPPORT			"005"

/** @brief   "<client> <user modes>" */
# define RPL_UMODEIS			"221"

/** @brief   "<client> :There are <u> users and <i> invisible on <s> servers" */
# define RPL_LUSERCLIENT		"251"

/** @brief   "<client> <ops> :operator(s) online" */
# define RPL_LUSEROP			"252"

/** @brief   "<client> <connections> :unknown connection(s)" */
# define RPL_LUSERUNKNOWN		"253"

/** @brief   "<client> <channels> :channels formed" */
# define RPL_LUSERCHANNELS		"254"

/** @brief   "<client> :I have <c> clients and <s> servers" */
# define RPL_LUSERME			"255"

/** @brief   "<client> [<u> <m>] :Current local users <u>, max <m>" */
# define RPL_LOCALUSERS			"265"

/** @brief   "<client> [<u> <m>] :Current global users <u>, max <m>" */
# define RPL_GLOBALUSERS		"266"

/** @brief   "<client> :[<reply>{ <reply>}]" */
# define RPL_MOTD				"302"

/** @brief   "<client> <channel> <modestring> <mode arguments>..." */
# define RPL_CHANNELMODEIS		"324"

/** @brief   "<client> <channel> <creationtime>" */
# define RPL_CREATIONTIME		"329"

/** @brief   "<client> <channel> :No topic is set" */
# define RPL_NOTOPIC			"331"

/** @brief   "<client> <channel> :<topic>" */
# define RPL_TOPIC				"332"

/** @brief   "<client> <channel> <nick> <setat>" */
# define RPL_TOPICWHOTIME		"333"

/** @brief   "<client> <nick> <channel>" */
# define RPL_INVITING			"341"

/** @brief   "<client> <symbol> <channel> :[prefix]<nick>{ [prefix]<nick>}" */
# define RPL_NAMREPLY			"353"

/** @brief   "<client> <channel> :End of /NAMES list" */
# define RPL_ENDOFNAMES			"366"

/** @brief   "<client> :- <server> Message of the day - " */
# define RPL_MOTDSTART			"375"

/** @brief   "<client> :End of /MOTD command." */
# define RPL_ENDOFMOTD			"376"

/** @brief   "<client> <nickname> :No such nick/channel" */
# define ERR_NOSUCHNICK			"401"

/** @brief   "<client> <server name> :No such server" */
# define ERR_NOSUCHSERVER		"402"

/** @brief   "<client> <channel> :No such channel" */
# define ERR_NOSUCHCHANNEL		"403"

/** @brief   "<client> <channel> :Cannot send to channel" */
# define ERR_CANNOTSENDTOCHAN	"404"
/** @brief   "<client> <channel> :You have joined too many channels" */
# define ERR_TOOMANYCHANNELS	"405"

/** @brief   "<client> :No recipient given (<command>)" */
# define ERR_NORECIPIENT		"411"

/** @brief   "<client> :No text to send" */
# define ERR_NOTEXTTOSEND		"412"

/** @brief   "<client> :MOTD File is missing" */
# define ERR_NOMOTD				"422"

/** @brief   "<client> :No nickname given" */
# define ERR_NONICKNAMEGIVEN	"431"

/** @brief   "<client> <nick> :Erroneus nickname" */
# define ERR_ERRONEUSNICKNAME   "432"

/** @brief   "<client> <nick> :Nickname is already in use" */
# define ERR_NICKNAMEINUSE      "433"

/** @brief   "<client> <nick> <channel> :They aren't on that channel" */
# define ERR_USERNOTINCHANNEL	"441"

/** @brief   "<client> <channel> :You're not on that channel" */
# define ERR_NOTONCHANNEL		"442"

/** @brief   "<client> <nick> <channel> :is already on channel" */
# define ERR_USERONCHANNEL		"443"

/** @brief   "<client> :You have not registered" */
# define ERR_NOTREGISTERED		"451"

/** @brief "<client> <command> :Not enough parameters" */
# define ERR_NEEDMOREPARAMS     "461"

/** @brief   "<client> :You may not reregister" */
# define ERR_ALREADYREGISTERED  "462"

/** @brief   "<client> :Password incorrect" */
# define ERR_PASSWDMISMATCH     "464"

/** @brief   "<client> <channel> :Cannot join channel (+l)" */
# define ERR_CHANNELISFULL		"471"

/** @brief   "<client> <modechar> :is unknown mode char to me" */
# define ERR_UNKNOWNMODE		"472"

/** @brief   "<client> <channel> :Cannot join channel (+i)" */
# define ERR_INVITEONLYCHAN		"473"

/** @brief   "<client> <channel> :Cannot join channel (+k)" */
# define ERR_BADCHANNELKEY		"475"

/** @brief   "<channel> :Bad Channel Mask" */
# define ERR_BADCHANMASK		"476"

/** @brief   "<client> <channel> :You're not channel operator" */
# define ERR_CHANOPRIVSNEEDED	"482"

/** @brief   "<client> :Unknown MODE flag" */
# define ERR_UMODEUNKNOWNFLAG	"501"

/** @brief   "<client> :Cant change mode for other users" */
# define ERR_USERSDONTMATCH		"502"

#endif
/* ************************************************************************** */