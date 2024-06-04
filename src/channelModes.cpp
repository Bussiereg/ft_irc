#include "Server.hpp"

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
	channel->setChannelKey(key);
	channel->setChannelMode('k', isOperator);
}

void Server::_modeOperatorPriv(bool isOperator, std::string ope, Client &client, Channel *channel)
{
	std::string const & nick = client.getNickname();
	std::string const & channelName = channel->getChannelName();
	std::map<Client *, bool> const &channelUsers = channel->getClientList();
	std::map<Client *, bool>::const_iterator subject = channelUsers.find(&client);

	if (subject == channelUsers.end())
	{
		client.appendResponse(ERR_NOTONCHANNEL(_serverName, nick, channelName));
		return;
	}

	if (subject->second == false)
	{
		client.appendResponse(ERR_CHANOPRIVSNEEDED(_serverName, nick, channelName));
		return;
	}

	std::map<Client *, bool>::iterator object;
	for (object = channel->getClientList().begin(); object != channel->getClientList().end(); ++object)
	{
		if (object->first->getNickname() == ope)
		{
			channel->setClientList(object->first, isOperator);
			client.appendResponse(MODE(nick, client.getUsername(), client.getHostname(), channelName, (isOperator ? "+o" : "-o"), ope));
			channel->relayMessage(client, MODE(nick, client.getUsername(), client.getHostname(), channelName, (isOperator ? "+o" : "-o"), ope));
			return;
		}
	}
	client.appendResponse(ERR_USERNOTINCHANNEL(_serverName, nick, ope, channelName));
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

	std::string const & channelName = params[1];
	std::vector<Channel*>::iterator it = find_if(_channelList.begin(), _channelList.end(), MatchChannelName(channelName));

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

	Channel *channel = *it;
	std::string const & modeCode = params[2];
	bool isModeOn = modeCode[0] == '+';

	for (unsigned int i = 1; i < modeCode.size(); i++)
	{
		char m = modeCode[i];
		if (params.size() < 4 && (m == 'o' || (isModeOn && (m == 'k' || m == 'l'))))
		{
			client.appendResponse(ERR_NEEDMOREPARAMS(_serverName, nick, "MODE"));
			continue ;
		}

		if (m == 'i')
			_modeInviteOnly(isModeOn, *channel);
		else if (m == 't')
			_modeTopic(isModeOn, *channel);
		else if (m == 'k')
			_modeKeySet(isModeOn, (isModeOn ? params[3] : ""), channel);
		else if (m == 'o')
			_modeOperatorPriv(isModeOn, params[3], client, channel);
		else if (m == 'l')
			_modeSetUserLimit(isModeOn, (isModeOn ? params[3] : ""), *channel);
		else
			client.appendResponse(ERR_UNKNOWNMODE(_serverName, client.getNickname(), std::string(1, m)));
	}
	channel->relayMessage(client, RPL_CHANNELMODEIS(_serverName, nick, channelName, channel->getModeString()));
	client.appendResponse(RPL_CHANNELMODEIS(_serverName, nick, channelName, channel->getModeString()));
}
