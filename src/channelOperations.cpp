#include "Server.hpp"

void Channel::getUserListInChannel(std::string &usersInChannel)
{
	std::map<Client *, bool>::iterator it;
	for (it = getClientList().begin(); it != getClientList().end(); ++it)
	{
		if (it->second)
			usersInChannel += '@';
		usersInChannel += it->first->getNickname() + " ";
	}
}

void Server::_createJoinmap(Client &client, std::string &message, std::map<std::string, std::string> &joinParams)
{
	std::vector<std::string> joinSplit = _splitString(message, ' ');
	std::string const &nick = client.getNickname();
	if (joinSplit.size() < 2)
	{
		client.appendResponse(ERR_NEEDMOREPARAMS(_serverName, nick, "JOIN"));
	}
	else if (joinSplit.size() == 2)
	{
		std::vector<std::string> channelList = _splitString(joinSplit[1], ',');
		for (unsigned long i = 0; i < channelList.size(); i++)
		{
			joinParams.insert(std::pair<std::string, std::string>(channelList[i], ""));
		}
	}
	else
	{
		std::vector<std::string> channelList = _splitString(joinSplit[1], ',');
		std::vector<std::string> channelKey = _splitString(joinSplit[2], ',');
		for (unsigned long i = 0; i < channelList.size(); i++)
		{
			if (channelKey.size() > i)
				joinParams.insert(std::pair<std::string, std::string>(channelList[i], channelKey[i]));
			else
				joinParams.insert(std::pair<std::string, std::string>(channelList[i], ""));
		}
	}
}

std::vector<Channel *> Server::getChannelList()
{
	return _channelList;
}

void Server::_handleTopicCommand(Client &client, std::string &input)
{
	std::string const &nick = client.getNickname();

	std::vector<std::string> paramTopic = _splitString(input, ' ');
	if (paramTopic.size() < 2)
	{
		client.appendResponse(ERR_NEEDMOREPARAMS(_serverName, nick, "TOPIC"));
		return;
	}
	std::vector<Channel *>::iterator it;
	for (it = _channelList.begin(); it != _channelList.end(); ++it)
	{
		if ((*it)->getChannelName() != paramTopic[1])
			continue ;
		
		if (!(*it)->isMember(client))
		{
			client.appendResponse(ERR_NOTONCHANNEL(_serverName, nick, (*it)->getChannelName()));
			return;
		}

		std::string const & channelName = (*it)->getChannelName();
		std::string const & topic = (*it)->getTopic();
		if (paramTopic.size() == 2)
		{
			if (topic.empty())
				client.appendResponse(RPL_NOTOPIC(_serverName, nick, channelName));
			else
				client.appendResponse(RPL_TOPIC(_serverName, nick, channelName, topic));
		}
		else if (paramTopic.size() >= 3)
		{
			if (((*it)->getChannelMode()['t'] == false) || (((*it)->getChannelMode()['t'] == true) && ((*it)->getClientList()[&client] == true)))
			{
				size_t colonpos = input.find(':');
				std::string newTopic = input.substr(colonpos + 1);
				(*it)->setTopic(newTopic);
				client.appendResponse(RPL_TOPIC(_serverName, nick, channelName, topic));
				(*it)->relayMessage(client, RPL_TOPIC(_serverName, nick, channelName, topic));
			}
		}
	}
}

unsigned int Server::_findMode(char m)
{
	char setMode[] = "itkol";
	for (int i = 0; setMode[i] != '\0'; i++)
	{
		if (m == setMode[i])
			return i;
	}
	return 5;
}

void Server::_modeInviteOnly(bool isOperator, Channel &channel)
{
	channel.setChannelMode('i', isOperator);
}

void Server::_modeTopic(bool isOperator, Channel &channel)
{
	channel.setChannelMode('t', isOperator);
}

void Server::_modeKeySet(bool isOperator, std::string key, Channel *channel)
{
	if (key.empty())
		channel->setChannelKey("");
	else
		channel->setChannelKey(key);
	channel->setChannelMode('k', isOperator);
}

void Server::_modeOperatorPriv(bool isOperator, std::string ope, Client &client, Channel *channel)
{
	std::map<Client *, bool> const &channelUsers = channel->getClientList();
	std::map<Client *, bool>::const_iterator subject = channelUsers.find(&client);

	if (subject == channelUsers.end())
	{
		return client.appendResponse(ERR_NOTONCHANNEL(_serverName, client.getNickname(), channel->getChannelName()));
	}

	if (subject->second == false)
	{
		return client.appendResponse(ERR_CHANOPRIVSNEEDED(_serverName, client.getNickname(), channel->getChannelName()));
	}

	std::map<Client *, bool>::iterator object;
	for (object = channel->getClientList().begin(); object != channel->getClientList().end(); ++object)
	{
		if (object->first->getNickname() == ope)
		{
			channel->setClientList(object->first, isOperator);
			return;
		}
	}
	client.appendResponse(ERR_USERNOTINCHANNEL(_serverName, client.getNickname(), ope, channel->getChannelName()));
}

void Server::_modeSetUserLimit(bool isOperator, std::string limit, Channel &channel)
{
	if (!limit.empty())
		channel.setChannelLimit(std::atoi(limit.c_str()));
	channel.setChannelMode('l', isOperator);
}

void Server::_handleModeCommand(Client &client, std::string &input)
{
	std::string const &nick = client.getNickname();
	std::vector<std::string> params = _splitString(input, ' ');

	if (params.size() < 2)
	{
		client.appendResponse(ERR_NEEDMOREPARAMS(_serverName, nick, "MODE"));
		return ;
	}

	std::vector<Channel*>::iterator it = find_if(_channelList.begin(), _channelList.end(), MatchChannelName(params[1]));

	if (it == _channelList.end())
	{
		client.appendResponse(ERR_NOSUCHCHANNEL(_serverName, nick, params[1]));
		return ;
	}

	if (params.size() == 2)
	{
		client.appendResponse(RPL_CHANNELMODEIS(_serverName, nick, (*it)->getChannelName(), (*it)->getModeString()));
		return ;
	}

	if ((*it)->isOperator(client) == false)
	{
		client.appendResponse(ERR_CHANOPRIVSNEEDED(_serverName, nick, (*it)->getChannelName()));
		return ;
	}

	if (params.size() == 3 && params[2].size() == 1 && params[2][0] == 'b') // silence the return ban of the client
		return;
	
	if (params[2][0] != '+' && params[2][0] != '-')
	{
		client.appendResponse(ERR_UNKNOWNMODE(_serverName, nick, params[2]));
		return;
	}

	Channel *channelInUse = *it;
	bool isModeOn = params[2][0] == '+';

	for (unsigned int i = 1; i < params[2].size(); i++)
	{
		char m = params[2][i];
		if (params.size() < 4)
		{
			if (m == 'o' || (isModeOn && (m == 'k' || m == 'l'))) {
				client.appendResponse(ERR_NEEDMOREPARAMS(_serverName, nick, "MODE"));
				return ;
			}
		}
		
		switch (_findMode(m))
		{
		case INVITE_ONLY:
			_modeInviteOnly(isModeOn, *channelInUse);
			break;
		case TOPIC:
			_modeTopic(isModeOn, *channelInUse);
			break;
		case KEY:
			if (isModeOn)
			{
				_modeKeySet(isModeOn, params[3], channelInUse);
			}
			else
			{
				_modeKeySet(isModeOn, "", channelInUse);
			}
			break;
		case OPERATOR_PRIVILEGE:
			_modeOperatorPriv(isModeOn, params[3], client, channelInUse);
			break;
		case LIMIT:
			if (isModeOn)
				_modeSetUserLimit(isModeOn, params[3], *channelInUse);
			else
				_modeSetUserLimit(isModeOn, "", *channelInUse);
			break;
		default:
			client.appendResponse(ERR_UNKNOWNMODE(_serverName, client.getNickname(), std::string(1, m)));
			return;
		}
	}
	(*it)->relayMessage(client, RPL_CHANNELMODEIS(_serverName, nick, channelInUse->getChannelName(), channelInUse->getModeString()));
	client.appendResponse(RPL_CHANNELMODEIS(_serverName, nick, channelInUse->getChannelName(), channelInUse->getModeString()));
}

void Server::_handleInviteCommand(Client &client, std::string &input)
{
	std::string const &nick = client.getNickname();
	std::vector<std::string> params = _splitString(input, ' ');

	if (params.size() < 3)
	{
		client.appendResponse(ERR_NEEDMOREPARAMS(_serverName, nick, "INVITE"));
		return;
	}

	std::string const & invitee = params[1];
	std::string const & channelName = params[2];

	std::vector<Channel*>::iterator it = find_if(_channelList.begin(), _channelList.end(), MatchChannelName(channelName));
	if (it == _channelList.end())
	{
		client.appendResponse(ERR_NOSUCHCHANNEL(_serverName, nick, channelName));
		return;
	}

	Channel *channel = *it;

	if (channel->isMember(client) == false)
	{
		client.appendResponse(ERR_NOTONCHANNEL(_serverName, nick, channelName));
		return;
	}

	if (channel->isOperator(client) == false)
	{
		client.appendResponse(ERR_CHANOPRIVSNEEDED(_serverName, nick, channelName));
		return;
	}

	std::vector<Client*>::iterator clientInvitee = find_if(_clients.begin(), _clients.end(), MatchNickname(invitee));
	if (clientInvitee == _clients.end())
	{
		client.appendResponse(ERR_NOSUCHNICK(_serverName, nick));
		return;
	}

	channel->setInviteList(invitee);
	client.appendResponse(RPL_INVITING(_serverName, nick, channelName, invitee));
}

void Server::_handleKickCommand(Client &client, std::string &input)
{
	std::string const &nick = client.getNickname();
	std::vector<std::string> params = _splitString(input, ' ');
	if (params.size() < 3)
	{
		client.appendResponse(ERR_NEEDMOREPARAMS(_serverName, nick, "KICK"));
		return ;
	}

	std::string const & channelName = params[1];
	std::string const & kickNick = params[2];

	std::string comment = "";
	if (params.size() >= 4)
	{
		size_t colonpos = input.find(':');
		comment = input.substr(colonpos + 1);
	}

	std::vector<Channel*>::iterator it = find_if(_channelList.begin(), _channelList.end(), MatchChannelName(channelName));
	if (it == _channelList.end())
	{
		client.appendResponse(ERR_NOSUCHCHANNEL(_serverName, nick, channelName));
		return;
	}

	Channel *channel = *it;
	Client *clientToBeKicked = _findClient(kickNick);
	if (clientToBeKicked == nullptr)
	{
		client.appendResponse(ERR_NOSUCHNICK(_serverName, nick));
		return;
	}
	if (channel->isMember(*clientToBeKicked) == false)
	{
		client.appendResponse(ERR_NOTONCHANNEL(_serverName, nick, channelName));
		return;
	}
	if (channel->isOperator(client) == false)
	{
		client.appendResponse(ERR_CHANOPRIVSNEEDED(_serverName, nick, channelName));
		return;
	}
	if (channel->isMember(kickNick) == false)
	{
		client.appendResponse(ERR_USERNOTINCHANNEL(_serverName, nick, kickNick, channelName));
		return;
	}
	channel->relayMessage(client, KICK(nick, client.getUsername(), client.getHostname(), channelName, kickNick, comment));
	client.appendResponse(KICK(nick, client.getUsername(), client.getHostname(), channelName, kickNick, comment));
	channel->removeClient(clientToBeKicked);
	clientToBeKicked->removeChannelJoined(channel);
}

void Server::_handlePartCommand(Client &client, std::string &input)
{
	std::string const &nick = client.getNickname();
	std::vector<std::string> params = _splitString(input, ' ');
	if (params.size() < 2)
	{
		std::string part = "PART";
		return client.appendResponse(ERR_NEEDMOREPARAMS(_serverName, nick, "PART"));
	}

	std::vector<std::string> channelList = _splitString(params[1], ',');

	std::string reason;
	if (params.size() >= 3 && params[2].size() > 1 && params[2][0] == ':')
	{
		reason = input.substr(input.find(':') + 1);
	}

	for (size_t i = 0; i < channelList.size(); i++)
	{
		std::string const & channelName = channelList[i];
		std::vector<Channel*>::iterator it = std::find_if(_channelList.begin(), _channelList.end(), MatchChannelName(channelName));
		if (it == _channelList.end())
		{
			client.appendResponse(ERR_NOSUCHCHANNEL(_serverName, nick, channelName));
			continue;
		}

		Channel *channel = *it;
		if (channel->isMember(client) == false)
		{
			client.appendResponse(ERR_NOTONCHANNEL(_serverName, nick, channelName));
			continue;
		}

		std::string const & username = client.getUsername();
		std::string const & hostname = client.getHostname();

		channel->relayMessage(client, PART(nick, username, hostname, channelName, reason));
		client.appendResponse(KICK(nick, username, hostname, channelName, nick, reason));
		channel->removeClient(&client);
		client.removeChannelJoined(channel);
		if (channel->getClientList().empty())
		{
			_channelList.erase(it);
			delete channel;
		}
	}
}
