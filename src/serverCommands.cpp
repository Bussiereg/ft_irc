#include "Server.hpp"

void Server::_readBuffer(size_t index, std::string & buffer)
{
	Client &client = *_clients[index - 1];
	std::string message;

	while (!(message = _getNextLine(buffer)).empty())
	{
		std::cout << "[Client " << index << "] Message received from client " << std::endl;
		std::cout << "     FD " <<_allSockets[index].fd << "< " << CYAN << message << RESET << std::endl;

		std::string command = _getCommand(message);
		(this->*_commandMap[command])(client, message);
	}

	std::cout << std::endl << "***list of clients***\n*" << std::endl;
	for (size_t j = 0; j < _clients.size(); j++)
	{
		std::cout << "* _clients[" << j << "].fd = " << _clients[j]->getClientSocket()->fd << "  ";
		std::cout << "_allSockets[" << j + 1 << "].fd = " << _allSockets[j + 1].fd << std::endl;;
	}
	std::cout << "*\n*********************" << std::endl << std::endl;
}

std::string Server::_getCommand(std::string & message)
{
	size_t pos = message.find(" ");
	std::string command = message.substr(0, pos);
	std::map<std::string, CommandFunction>::iterator it = _commandMap.find(command);
	return it != _commandMap.end() ? command : "INVALID";
}

void Server::_handlePassCommand(Client & client, std::string & message)
{
	std::vector<std::string> params = _splitString(message, ' ');
	if (client.isPassedWord())
		client.appendResponse("462 :You may not reregister\r\n");
	else if (client.passWordAttempted())
		client.appendResponse("you only get one password try\r\n");
	else if (params.size() != 2)
		client.appendResponse("wrong amount of arguments\r\n");
	else if (params[1] != _password)
		client.appendResponse("464 :Password incorrect\r\n");
	else {
		std::cout << "[Server  ] Password accepted for " << client.getNickname() << std::endl;
		client.acceptPassword();
	}
	client.passWordAttempt();
}

void Server::_handleNickCommand(Client & client, std::string & message)
{
	if (!client.isPassedWord())
	{
		client.appendResponse("No password given as first command");
		client.passWordAttempt();
		return ;
	}
	if (message.length() < 6) {
		client.appendResponse(ERR_NONICKNAMEGIVEN(_serverName));
		return ;
	}
	std::string nickname = message.substr(5);
	if (_isNickInUse(nickname))
		client.appendResponse(ERR_NICKNAMEINUSE(nickname));
	else if (client.getNickname().empty())
	{
		std::cout << "[Server  ] Nickname accepted for " << nickname << std::endl;
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
	std::vector<std::string> params = _splitString(message, ' ');
	std::string username = message.substr(5);
	if (client.getNickname().empty())
		return;
	else if (!client.isPassedWord())
	{
		client.appendResponse("No password given as first command");
		client.passWordAttempt();
	}
	else if (params.size() < 2)
	{
		client.appendResponse(":localhost 461 " + client.getNickname() + " USER :Not enough parameters\r\n");
	}
	else
	{
		client.setUsername(params[1]);
		if (params.size() > 4)
			client.setHostname(params[3]);
		if (params.size() > 5)
			client.setRealname(concatenateTokens(params, 4));
		if (!client.isFullyAccepted()) {
			std::cout << "[Server  ] Username accepted for " << client.getNickname() << std::endl;
			client.acceptFully();
			client.appendResponse(RPL_WELCOME(client.getNickname(), client.getUsername(), "localhost"));
		}
	}
}

void Server::_handlePrivmsgCommand(Client & client, std::string & message)
{
	std::vector<std::string> receivers = _parseReceivers(message);
	if (receivers.empty()) {
		client.appendResponse(ERR_NORECIPIENT(_serverName, client.getNickname(), message.substr(0,7)));
		return ;
	}
	size_t pos = message.find(" :");
	if (pos == std::string::npos) {
		client.appendResponse(ERR_NOTEXTTOSEND(_serverName, client.getNickname()));
		return ;
	}
	std::string forward = client.getNickname() + " :" + message.substr(pos + 2) + "\r\n";
	for (std::vector<std::string>::iterator it = receivers.begin(); it != receivers.end(); ++it) {
		std::vector<Client*>::iterator ite = _clients.begin();
		for (; ite != _clients.end(); ++ite)
			if ((*ite)->getNickname() == *it) {
				(*ite)->appendResponse(forward);
				break ;
			}
		if (ite == _clients.end()) {
			std::vector<Channel>::iterator iter = _channelList.begin();
			for (; iter != _channelList.end(); ++iter) {
				if (iter->getChannelName() == *it) {
					iter->relayMessage(client, message);
					std::cout << "Server relayed the message" << std::endl;
					break ;
				}
			}
			if (iter == _channelList.end())
				client.appendResponse(ERR_NOSUCHNICK(_serverName, client.getNickname(), *it));
		}
	}
}

void Server::_handlePingCommand(Client & client, std::string & message)
{
	// should check if servername is given, otherwise error
	client.appendResponse(PONG(message.substr(5)));
}

void Server::_handleQuitCommand(Client & client, std::string & message)
{
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (*it != &client)
		{
			std::vector<int>::const_iterator ContactFDListBegin = (*it)->getContactList().begin();
			std::vector<int>::const_iterator ContactFDListEnd = (*it)->getContactList().begin();
			if (std::find(ContactFDListBegin, ContactFDListEnd, (*it)->getClientSocket()->fd) != ContactFDListEnd)
				(*it)->appendResponse(QUIT_REASON(client.getNickname(), client.getUsername(), client.getHostname(), message.substr(6)));
			//else if ( both client are part of the same channel)
			// {
			//	Channel Notifications: Users in the same channels as the quitting user receive a notification. 
			// 	Channel Member List: The user's name will be removed from the list of channel members.
			// 	Potential Channel Events: Depending on the server configuration and channel settings, 
			// 	additional events might be triggered. For example, if the user held special privileges
			// 	(like being an operator), the server might handle the redistribution of those privileges.
			// }
		}
	}
	_delClient(client);
}

void Server::_handleInvalidCommand(Client & client, std::string & message)
{
	size_t pos = message.find(" ");
	std::string command = message.substr(0, pos);
	client.appendResponse(ERR_UNKNOWNCOMMAND(_serverName, client.getNickname(), command));
}

void Server::_handleCapCommand(Client &, std::string &){

}