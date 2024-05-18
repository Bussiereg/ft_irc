/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverCommands.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 16:49:06 by mwallage          #+#    #+#             */
/*   Updated: 2024/05/18 17:19:19 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

void Server::_readBuffer(size_t index, std::string & buffer)
{
	Client &client = *_clients[index - 1];
	std::string response;
	std::string message;

	while (!(message = _getNextLine(index, buffer)).empty())
	{
		std::cout << "[Client] Message received from client " << client.getClientSocket()->fd << std::endl;
		std::cout << _allSockets[index].fd << " << " << message << std::endl;

		enum Commands commandCase = _getCommand(message);
		switch (commandCase) {
			case PASS:
				response = _handlePassCommand(client, message);
				break;
			case NICK:
				response = _handleNickCommand(client, message);
				break;
			case USER:
				response = _handleUserCommand(client, message);
				break;
			case PRIVMSG:
				response = _handlePrivmsgCommand(client, message);
				break;
			case INVALID:
				response = client.getNickname() + ' ' + message + " :Unknown command\r\n";
		}

		if (!response.empty() && _allSockets[index].revents & POLLOUT) {
			send(_allSockets[index].fd, response.c_str(), response.size(), 0);
		}
	}

	std::cout << std::endl << "***list of clients***" << std::endl;
	for (size_t j = 0; j < _clients.size(); j++)
	{
		std::cout << "_clients[" << j << "].fd = " << _clients[j]->getClientSocket()->fd << std::endl;
	}
	std::cout << "***" << std::endl << std::endl;
}

Commands Server::_getCommand(std::string & message)
{
	if (message.find("PASS") == 0) {
		return PASS;
	} else if (message.find("NICK") == 0) {
		return NICK;
	} else if (message.find("USER") == 0) {
		return USER;
	} else if (message.find("PRIVMSG") == 0) {
		return PRIVMSG;
	}
	return INVALID;
}

std::string Server::_handlePassCommand(Client & client, std::string & message)
{
	if (client.isPassedWord()) {
		return ("462 :You may not reregister\r\n");
	}

	if (message.substr(5) != _password)
	{
		return ("464 :Password incorrect\r\n");
	}
	std::cout << "[Server] Password accepted for " << client.getNickname() << std::endl;
	client.acceptPassword();
	return "";
}

std::string Server::_handleNickCommand(Client & client, std::string & message)
{
	std::string response;
	std::string nickname = message.substr(5);
	if (nickname.empty())
		return ERR_NONICKNAMEGIVEN;
	if (_isNickInUse(nickname))
        return ERR_NICKNAMEINUSE(nickname);
	if (client.getNickname().empty()) {
		std::cout << "[Server] Nickname accepted for " << nickname << std::endl;
		client.setNickname(nickname);
		return "";
	}
	std::string oldNick = client.getNickname();
	client.setNickname(nickname);
	return RPL_NICK(oldNick, nickname);
}

std::string Server::_handleUserCommand(Client & client, std::string & message)
{
	std::string username = message.substr(5);
	if (username.empty())
	{
		return ":localhost 461 " + client.getNickname() + " USER :Not enough parameters\r\n";
	}
	client.setUsername(username);
	if (!client.isFullyAccepted()) {
		std::cout << "[Server] Username accepted for " << client.getNickname() << std::endl;
		client.acceptFully();
		return RPL_WELCOME(client.getNickname(), client.getUsername(), "localhost");
	} else {
		return ":localhost 001 * :Username accepted\r\n";
	}
}

std::string Server::_handlePrivmsgCommand(Client & , std::string & message)
{
	return message;
}