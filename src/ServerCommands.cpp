#include "Server.hpp"

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
		std::string response = ":" + oldNick + "!" + client.getUsername() + "@" + client.getHostname() + " NICK " + nick;
		client.appendResponse(response);

		// Forwarded to every client in the same channel:
		for (std::vector<Channel *>::iterator it = _channelList.begin(); it != _channelList.end(); ++it) {
			// the message should have the right form so that in irssi it will appear in the relevant channel
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
		client.appendResponse(ERR_NORECIPIENT(_serverName, nick, "PRIVMSG"));
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
		std::string forward = ":" + nick + "!" + nick + "@" + client.getHostname() + " PRIVMSG " + *it + " :" + message.substr(pos + 2);
		if ((*it)[0] == '#')
		{
			std::vector<Channel *>::iterator ite = std::find_if(_channelList.begin(), _channelList.end(), MatchChannelName(*it));
			if (ite == _channelList.end())
				client.appendResponse(ERR_NOSUCHNICK(_serverName, nick));
			else if (!(*ite)->isMember(client))
				client.appendResponse(ERR_NOTONCHANNEL(_serverName, nick, (*ite)->getChannelName()));
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
			}
		}
	}
}

void Server::_handlePingCommand(Client &client, std::string &message)
{
	client.appendResponse("PONG " + message.substr(5));
}

void Server::_handleQuitCommand(Client &client, std::string &message)
{
	std::vector<Channel*> channelList = client.getChannelJoined();
	std::string reason = "";
	if (message.size() > 6)
		reason += message.substr(6);
	std::vector<Channel*>::iterator it;
	for (it = channelList.begin(); it != channelList.end(); ++it) {
		(*it)->relayMessage(client, PART(client.getNickname(), client.getUsername(), client.getHostname(), (*it)->getChannelName(), reason));
	}
	_removeClient(client);
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
	if (params.size() < 2)
	{
		client.appendResponse(ERR_NEEDMOREPARAMS(_serverName, client.getNickname(), "WHO"));
		return;
	}

	std::string const & channelMask = params[1];
	for (std::vector<Channel *>::iterator it_ch = _channelList.begin(); it_ch != _channelList.end(); ++it_ch)
	{
		if ((*it_ch)->getChannelName() == channelMask)
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

void Server::_handleMotdCommand(Client & client, std::string & )
{
	std::string filename(_config.get("Misc", "Motd"));
	std::ifstream motd_file(filename.c_str());
	std::string nick = client.getNickname();

	if (filename.empty() || !motd_file.is_open()) {
		client.appendResponse(ERR_NOMOTD(_serverName, nick));
		return ;
	}
	
	client.appendResponse(RPL_MOTDSTART(_serverName, nick));
	std::string line;
	while(std::getline(motd_file, line)) {
		client.appendResponse(RPL_MOTD(_serverName, nick) + line);
	}
	client.appendResponse(RPL_ENDOFMOTD(_serverName, nick));
}