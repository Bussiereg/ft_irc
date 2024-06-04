#include "Server.hpp"

std::vector<Channel *> Server::getChannelList()
{
	return _channelList;
}

void Server::_handleTopicCommand(Client &client, std::string &input)
{
	std::string const &nick = client.getNickname();

	std::vector<std::string> params = _splitString(input, ' ');
	if (params.size() < 2)
	{
		client.appendResponse(ERR_NEEDMOREPARAMS(_serverName, nick, "TOPIC"));
		return;
	}

	std::string const & channelName = params[1];
	std::vector<Channel *>::iterator it = find_if(_channelList.begin(), _channelList.end(), MatchChannelName(channelName));
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
	
	std::string const & topic = channel->getTopic();
	
	if (params.size() == 2)
	{
		if (topic.empty())
			client.appendResponse(RPL_NOTOPIC(_serverName, nick, channelName));
		else
			client.appendResponse(RPL_TOPIC(_serverName, nick, channelName, topic));
		return ;
	}
	
	if (channel->isModeOn('t') == false || (channel->isOperator(client) == true))
	{
		size_t colonpos = input.find(':');
		std::string newTopic = input.substr(colonpos + 1);
		channel->setTopic(newTopic);
		client.appendResponse(RPL_TOPIC(_serverName, nick, channelName, topic));
		channel->relayMessage(client, RPL_TOPIC(_serverName, nick, channelName, topic));
	}
	else
		client.appendResponse(ERR_CHANOPRIVSNEEDED(_serverName, nick, channelName));
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
