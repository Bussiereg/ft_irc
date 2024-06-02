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
	std::string const & nick = client.getNickname();

	if (client.isPassedWord())
		client.appendResponse(ERR_ALREADYREGISTRED(_serverName, nick));
	else if (params.size() < 2 || params[1] != _password)
		client.appendResponse(ERR_PASSWDMISMATCH(_serverName, nick));
	else
	{
		std::cout << "[Server  ] Password accepted for " << client.getNickname() << std::endl;
		client.acceptPassword();
	}
}

void Server::_handleNickCommand(Client &client, std::string &message)
{
	std::vector<std::string> params = _splitString(message, ' ');
	if (!client.isPassedWord()) {
		client.appendResponse(ERR_NOTREGISTERED(_serverName, ""));
		return;
	}
	if (params.size() < 2) {
		client.appendResponse(ERR_NONICKNAMEGIVEN(_serverName, ""));
		return;
	} 
	std::string nick = params[1];
	if (!_isValidNickname(nick)) {
		client.appendResponse(ERR_ERRONEUSNICKNAME(_serverName, client.getNickname(), nick));
		return ;
	}
	if (_isNickInUse(nick)) {
		client.appendResponse(ERR_NICKNAMEINUSE(_serverName, client.getNickname(), nick));
		return ;
	}
	std::string oldNick = client.getNickname();
	client.setNickname(nick);
	if (!oldNick.empty()) {
		std::string response = ":" + oldNick + "!" + client.getUsername() + "@" + client.getHostname() + " NICK " + nick + "\r\n";
		client.appendResponse(response);

		// Forwarded to every client in the same channel:
		for (std::vector<Channel *>::iterator it = _channelList.begin(); it != _channelList.end(); ++it) {
			if ((*it)->isMember(client))
				(*it)->relayMessage(client, response);
		}
	}
	std::cout << "[Server  ] Nickname accepted for " << nick << std::endl;

	if (!client.isFullyAccepted() && !client.getUsername().empty())
		_registerUser(client);
}

void Server::_handleUserCommand(Client &client, std::string &message)
{
	std::vector<std::string> params = _splitString(message, ' ');
	std::string username = message.substr(5);
	if (!client.isPassedWord()) {
		client.appendResponse(ERR_NOTREGISTERED(_serverName, client.getNickname()));
		return;
	}
	if (params.size() < 5)
	{
		client.appendResponse(ERR_NEEDMOREPARAMS(_serverName, client.getNickname(), "USER"));
		return ;
	}
	client.setUsername(params[1]);
	client.setHostname("localhost");
	client.setRealname(_concatenateTokens(params, 4));
	if (!client.isFullyAccepted() && !client.getNickname().empty())
		_registerUser(client);
}

void Server::_registerUser(Client &client)
{
	client.acceptFully();
	client.appendResponse(RPL_WELCOME(_serverName, client.getNickname(), client.getUsername(), client.getHostname()));
	std::string dummy_message;
	_handleMotdCommand(client, dummy_message);
}

void Server::_handlePrivmsgCommand(Client &client, std::string &message)
{
	std::string const & nick = client.getNickname();
	std::vector<std::string> receivers = _parseReceivers(message);
	if (receivers.empty())
	{
		client.appendResponse(ERR_NORECIPIENT(_serverName, nick, message.substr(0, 7)));
		return;
	}

	size_t pos = message.find(" :");
	if (pos == std::string::npos)
	{
		client.appendResponse(ERR_NOTEXTTOSEND(_serverName, nick));
		return;
	}

	for (std::vector<std::string>::iterator it = receivers.begin(); it != receivers.end(); ++it)
	{
		std::string forward = ":" + nick + "!" + client.getUsername() + "@" + client.getHostname() + " PRIVMSG " + *it + " :" + message.substr(pos + 2) + "\r\n";
		if ((*it)[0] == '#')
		{
			std::vector<Channel *>::iterator ite = std::find_if(_channelList.begin(), _channelList.end(), MatchChannelName(*it));
			if (ite == _channelList.end())
				client.appendResponse(ERR_NOSUCHNICK(_serverName, nick));
			else if (!(*ite)->isMember(client)){
				client.appendResponse(ERR_NOTONCHANNEL(_serverName, nick, (*ite)->getChannelName()));
			}
			else
				(*ite)->relayMessage(client, forward);
		}
		else
		{
			std::vector<Client *>::iterator ite = std::find_if(_clients.begin(), _clients.end(), MatchNickname(*it));
			if (ite == _clients.end())
				client.appendResponse(ERR_NOSUCHNICK(_serverName, nick));
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
	client.appendResponse("PONG " + message.substr(5) + "\r\n");
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
						client.appendResponse(RPL_WHO(_serverName, client.getNickname(), "#all" , (*it_cl).first->getUsername(), (*it_cl).first->getHostname(), (*it_cl).first->getNickname(), "*", (*it_cl).first->getRealname()));
					else
						client.appendResponse(RPL_WHO(_serverName, client.getNickname(), "#all" , (*it_cl).first->getUsername(), (*it_cl).first->getHostname(), (*it_cl).first->getNickname(), "", (*it_cl).first->getRealname()));
				}
				client.appendResponse(RPL_ENDWHO(_serverName, client.getNickname(), "#all"));
				break;
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

void Server::_handleMotdCommand(Client & client, std::string & )
{
	std::string filename(_config.get("Misc", "Motd"));
	std::ifstream motd_file(filename.c_str());
	std::string nick = client.getNickname();

	std::cout << "Filename motd: " << filename << std::endl;
	std::cout << "The file fd:\n" << motd_file << std::endl;

	if (filename.empty() || !motd_file.is_open())
		client.appendResponse(ERR_NOMOTD(_serverName, nick));
	else {
		client.appendResponse(RPL_MOTDSTART(_serverName, nick));
		std::string line;
		while(std::getline(motd_file, line)) {
			client.appendResponse(RPL_MOTD(_serverName, nick));
			client.appendResponse(line + "\r\n");
		}
		client.appendResponse(RPL_ENDOFMOTD(_serverName, nick));
	}
}