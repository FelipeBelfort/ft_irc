/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMessageTools.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 17:50:27 by jm                #+#    #+#             */
/*   Updated: 2024/05/10 17:51:05 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::string		Server::serverMessage(std::string src, \
	std::string cmd)
{
	std::string	line;

	if (src.empty())
		src.assign("*");
	if (cmd.empty())
		cmd.assign("*");
	line = ":" + src + " " + cmd + "\r\n";
	return (line);
}
std::string		Server::serverMessage(std::string src, \
	std::string cmd, std::string msg)
{
	std::string	line;

	if (src.empty())
		src.assign("*");
	if (cmd.empty())
		cmd.assign("*");
	if (msg.empty())
		msg.assign("");
	line = ":" + src + " " + cmd + " :" + msg + "\r\n";
	return (line);
}
std::string		Server::numericMessage(std::string src, \
	std::string num, std::string target, std::string msg)
{
	std::string	line;

	if (src.empty())
		src.assign("*");
	if (num.empty())
		num.assign("*");
	if (target.empty())
		target.assign("*");
	if (msg.empty())
		msg.assign("");
	line = ":" + src + " " + num + " " + target + " :" + msg + "\r\n";
	return (line);
}

std::string		Server::numericMessage(std::string src, \
	std::string num, std::string other_params)
{
	std::string	line;

	if (src.empty())
		src.assign("*");
	if (num.empty())
		num.assign("*");
	if (other_params.empty())
		other_params.assign("*");
	line = ":" + src + " " + num + " " + other_params + "\r\n";
	return (line);
}

std::string		Server::ErrorMessage(std::string msg)
{
	std::string	line;

	if (msg.empty())
		msg.assign(" ");
	line = "ERROR :" + msg + "\r\n";
	return (line);
}
