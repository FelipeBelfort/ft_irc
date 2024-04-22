/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Include.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 16:57:31 by jm                #+#    #+#             */
/*   Updated: 2024/04/22 20:50:00 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDE_H
# define INCLUDE_H

# define SERVERNAME             "ircserver"
# define HOSTNAME               "localhost"

# define RPL_WELCOME            "001"
# define RPL_YOURHOST           "002"
# define RPL_CREATED            "003"
# define RPL_MYINFO             "004"
# define RPL_ISUPPORT           "005"
# define RPL_LUSERCLIENT		"251"
# define RPL_LUSEROP			"252"
# define RPL_LUSERUNKNOWN		"253"
# define RPL_LUSERCHANNELS		"254"
# define RPL_LUSERME			"255"
# define RPL_LOCALUSERS			"265"
# define RPL_GLOBALUSERS		"266"
# define RPL_MOTD				"302"
# define RPL_MOTDSTART			"375"
# define RPL_ENDOFMOTD			"376"
# define ERR_NOSUCHSERVER		"402"
# define ERR_NOMOTD				"422"
# define ERR_NONICKNAMEGIVEN    "431"
# define ERR_ERRONEUSNICKNAME   "432"
# define ERR_NICKNAMEINUSE      "433"
# define ERR_NEEDMOREPARAMS     "461"
# define ERR_ALREADYREGISTERED  "462"
# define ERR_PASSWDMISMATCH     "464"

//TODO review macros below by making them dynamic with data information holders
# define CASEMAPPING			"CASEMAPPING=ascii"
# define CHANLIMIT				"CHANLIMIT=#&:42"
# define CHANNELLEN				"CHANNELLEN=24"
# define CHANTYPES				"CHANTYPES=&#"
# define CHANMODES				"CHANMODES=i,t,k,l"
# define PREFIX					"PREFIX=(o)@"
# define KICKLEN				"KICKLEN=242"
# define MODES					"MODES=5"
# define NICKLEN				"NICKLEN=24"
# define STATUSMSG				"STATUSMSG=@"
# define TOPICLEN				"TOPICLEN=242"
# define USERLEN				"USERLEN=24"

typedef enum    e_type
{
    _false, _true, _ignore, _fatal, _error
}               t_type;

#endif
/* ************************************************************************** */