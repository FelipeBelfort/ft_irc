/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Include.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 16:57:31 by jm                #+#    #+#             */
/*   Updated: 2024/04/19 11:54:35 by jm               ###   ########lyon.fr   */
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
# define ERR_NONICKNAMEGIVEN    "431"
# define ERR_ERRONEUSNICKNAME   "432"
# define ERR_NICKNAMEINUSE      "433"
# define ERR_NEEDMOREPARAMS     "461"
# define ERR_ALREADYREGISTERED  "462"
# define ERR_PASSWDMISMATCH     "464"
typedef enum    e_type
{
    _false, _true, ignore, _fatal, _error
}               t_type;

#endif
/* ************************************************************************** */