#include "Server.hpp"

void Server::_readBuffer(size_t index, std::string &buffer)
{
	Client &client = *_clients[index - 1];
	std::string message;

	while (!(message = _getNextLine(buffer)).empty())
	{
		std::cout << "[Client " << index << "] Message received from client " << std::endl;
		std::cout << "     FD " << _allSockets[index].fd << "< " << CYAN << message << RESET << std::endl;

		std::string command = _getCommand(message);
		(this->*_commandMap[command])(client, message);
	}

	std::cout << std::endl
			  << "***list of clients***\n*" << std::endl;
	for (size_t j = 0; j < _clients.size(); j++)
	{
		std::cout << "* _clients[" << j << "].fd = " << _clients[j]->getClientSocket()->fd << "  ";
		std::cout << "_allSockets[" << j + 1 << "].fd = " << _allSockets[j + 1].fd << std::endl;
		;
	}
	std::cout << "*\n*********************" << std::endl
			  << std::endl;
}

std::string Server::_getCommand(std::string &message)
{
	size_t pos = message.find(" ");
	std::string command = message.substr(0, pos);
	std::map<std::string, CommandFunction>::iterator it = _commandMap.find(command);
	return it != _commandMap.end() ? command : "INVALID";
}

void Server::_handlePassCommand(Client &client, std::string &message)
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
	else
	{
		std::cout << "[Server  ] Password accepted for " << client.getNickname() << std::endl;
		client.acceptPassword();
	}
	client.passWordAttempt();
}

void Server::_handleNickCommand(Client &client, std::string &message)
{
	if (!client.isPassedWord())
	{
		client.appendResponse("No password given as first command");
		client.passWordAttempt();
		return;
	}
	if (message.length() < 6)
	{
		client.appendResponse(ERR_NONICKNAMEGIVEN(_serverName));
		return;
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

void Server::_handleUserCommand(Client &client, std::string &message)
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
			client.setRealname(_concatenateTokens(params, 4));
		if (!client.isFullyAccepted())
		{
			std::cout << "[Server  ] Username accepted for " << client.getNickname() << std::endl;
			client.acceptFully();
			client.appendResponse(RPL_WELCOME(client.getNickname(), client.getUsername(), "localhost"));
		}
	}
}

void Server::_handlePrivmsgCommand(Client &client, std::string &message)
{
	std::vector<std::string> receivers = _parseReceivers(message);
	if (receivers.empty())
	{
		client.appendResponse(ERR_NORECIPIENT(_serverName, client.getNickname(), message.substr(0, 7)));
		return;
	}

	size_t pos = message.find(" :");
	if (pos == std::string::npos)
	{
		client.appendResponse(ERR_NOTEXTTOSEND(_serverName, client.getNickname()));
		return;
	}

	for (std::vector<std::string>::iterator it = receivers.begin(); it != receivers.end(); ++it)
	{
		std::string forward = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.gethostname() + " PRIVMSG " + *it + " :" + message.substr(pos + 2) + "\r\n";
		if ((*it)[0] == '#')
		{
			std::vector<Channel *>::iterator ite = std::find_if(_channelList.begin(), _channelList.end(), MatchChannelName(*it));
			if (ite == _channelList.end())
				client.appendResponse(ERR_NOSUCHNICK(_serverName, client.getNickname(), *it));
			else if (!(*ite)->isMember(client)){
				client.appendResponse(ERR_NOTONCHANNEL(_serverName, (*ite)->getChannelName()));
			}
			else
				(*ite)->relayMessage(client, forward);
		}
		else
		{
			std::vector<Client *>::iterator ite = std::find_if(_clients.begin(), _clients.end(), MatchNickname(*it));
			if (ite == _clients.end())
				client.appendResponse(ERR_NOSUCHNICK(_serverName, client.getNickname(), *it));
			else
			{
				(*ite)->appendResponse(forward);
				(*ite)->getContactList().insert(client.getClientSocket()->fd);
			}
		}
	}
}

void Server::_handlePingCommand(Client &client, std::string &message)
{
	client.appendResponse(PONG(message.substr(5)));
}

void Server::_handleQuitCommand(Client &client, std::string &message)
{
	for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (*it != &client)
		{
			if (std::find((*it)->getContactList().begin(), (*it)->getContactList().end(), client.getClientSocket()->fd) != (*it)->getContactList().end())
				(*it)->appendResponse(QUIT_REASON(client.getNickname(), client.getUsername(), client.getHostname(), message.substr(6)));
			else
			{
				for (std::vector<Channel *>::iterator it_ch = (*it)->getChannelJoined().begin(); it_ch != (*it)->getChannelJoined().end(); ++it_ch)
				{
					std::map<Client*, bool>::iterator it_find = (*it_ch)->getClientList().find(&client);
					 if (it_find != (*it_ch)->getClientList().end())
						(*it)->appendResponse(QUIT_REASON(client.getNickname(), client.getUsername(), client.getHostname(), message.substr(6)));
				}
			}
		}
	}
	_delClient(client);
}

void Server::_handleInvalidCommand(Client &client, std::string &message)
{
	size_t pos = message.find(" ");
	std::string command = message.substr(0, pos);
	client.appendResponse(ERR_UNKNOWNCOMMAND(_serverName, client.getNickname(), command));
}

void Server::_handleCapCommand(Client &, std::string &)
{
}

void Server::_handleWhoCommand(Client & client, std::string & message)
{
	std::vector<std::string> params = _splitString(message, ' ');
	bool found_channel = false;
	if (params.size() == 2)
	{
		for (std::vector<Channel *>::iterator it_ch = _channelList.begin(); it_ch != _channelList.end(); ++it_ch)
		{
			if ((*it_ch)->getChannelName() == params[1])
			{
				found_channel = true;
				for (std::map<Client*, bool>::iterator it_cl = (*it_ch)->getClientList().begin(); it_cl != (*it_ch)->getClientList().end(); ++it_cl)
				{
					if ((*it_cl).second == true)
						client.appendResponse(RPL_WHO(_serverName, client.getNickname(), (*it_ch)->getChannelName(), (*it_cl).first->getUsername(), (*it_cl).first->getHostname(), (*it_cl).first->getNickname(), "*", (*it_cl).first->getRealname()));
					else
						client.appendResponse(RPL_WHO(_serverName, client.getNickname(), (*it_ch)->getChannelName(), (*it_cl).first->getUsername(), (*it_cl).first->getHostname(), (*it_cl).first->getNickname(), "", (*it_cl).first->getRealname()));
				}
			client.appendResponse(RPL_ENDWHO(_serverName, client.getNickname(), (*it_ch)->getChannelName()));
			}
		}
		if (found_channel == false)
		{
			for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
				client.appendResponse(RPL_WHO(_serverName, client.getNickname(), "#all" , (*it)->getUsername(), (*it)->getHostname(), (*it)->getNickname(), "", (*it)->getRealname()));
		client.appendResponse(RPL_ENDWHO(_serverName, client.getNickname(), "#all"));
		}
	}
}
