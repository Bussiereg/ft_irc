/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverCommands.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 16:49:06 by mwallage          #+#    #+#             */
/*   Updated: 2024/05/18 18:49:46 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::_readBuffer(size_t index, std::string & buffer)
{
	Client &client = *_clients[index - 1];
	std::string message;

	while (!(message = _getNextLine(index, buffer)).empty())
	{
		std::cout << "[Client] Message received from client fd " << client.getClientSocket()->fd << std::endl;
		std::cout << "    " <<_allSockets[index].fd << " << " << CYAN << message << RESET << std::endl;

		enum Commands commandCase = _getCommand(message);
		switch (commandCase) {
			case PASS:
				_handlePassCommand(client, message);
				break;
			case NICK:
				_handleNickCommand(client, message);
				break;
			case USER:
				_handleUserCommand(client, message);
				break;
			case PRIVMSG:
				_handlePrivmsgCommand(client, message);
				break;
			case JOIN:
				_handleJoinCommand(client, message);
				break;
			case INVALID:
				client.appendResponse(client.getNickname() + ' ' + message + " :Unknown command\r\n");
		}
	}

	std::cout << std::endl << "***list of clients***" << std::endl;
	for (size_t j = 0; j < _clients.size(); j++)
	{
		std::cout << "_clients[" << j << "].fd = " << _clients[j]->getClientSocket()->fd << std::endl;
		std::cout << "_allSockets[" << j + 1 << "].fd = " << _allSockets[j + 1].fd << std::endl;;
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
	}else if (message.find("JOIN") == 0) {
		return JOIN;
	}
	return INVALID;
}

void Server::_handlePassCommand(Client & client, std::string & message)
{
	if (client.isPassedWord()) {
		client.appendResponse("462 :You may not reregister\r\n");
	} else if (message.substr(5) != _password) {
		client.appendResponse("464 :Password incorrect\r\n");
	} else {
		std::cout << "[Server] Password accepted for " << client.getNickname() << std::endl;
		client.acceptPassword();
	}
}

void Server::_handleNickCommand(Client & client, std::string & message)
{
	std::string response;
	std::string nickname = message.substr(5);
	if (nickname.empty())
		client.appendResponse(ERR_NONICKNAMEGIVEN);
	else if (_isNickInUse(nickname))
        client.appendResponse(ERR_NICKNAMEINUSE(nickname));
	else if (client.getNickname().empty()) {
		std::cout << "[Server] Nickname accepted for " << nickname << std::endl;
		client.setNickname(nickname);
	}
	else
	{
		std::string oldNick = client.getNickname();
		client.setNickname(nickname);
		client.appendResponse(RPL_NICK(oldNick, nickname));
	}
}


void Server::_handleUserCommand(Client & client, std::string & message)
{
	std::string username = message.substr(5);
	if (username.empty())
	{
		client.appendResponse(":localhost 461 " + client.getNickname() + " USER :Not enough parameters\r\n");
	}
	else 
	{
		client.setUsername(username);
		if (!client.isFullyAccepted()) {
			std::cout << "[Server] Username accepted for " << client.getNickname() << std::endl;
			client.acceptFully();
			client.appendResponse(RPL_WELCOME(client.getNickname(), client.getUsername(), "localhost"));
		}
	}
}

void Server::_handlePrivmsgCommand(Client & client, std::string & message)
{
	std::string forward = client.getNickname() + " :" + message.substr(8) + "\r\n";
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (*it != &client)
			(*it)->appendResponse(forward);
	}
}