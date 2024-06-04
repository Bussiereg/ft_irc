#include "Server.hpp"


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
