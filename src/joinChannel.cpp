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

void Server::_joinExistingChannel(Client &client, Channel &channel, std::string const &key)
{
	std::string const &nick = client.getNickname();
	std::map<char, bool> &channelMode = channel.getChannelMode();
	std::string const &channelName = channel.getChannelName();

	if (channelMode['k'] && key != channel.getChannelKey())
		client.appendResponse(ERR_BADCHANNELKEY(_serverName, nick, channelName));
	else if (channelMode['l'] && (channel.getLimitUsers() <= channel.getClientList().size()))
		client.appendResponse(ERR_CHANNELISFULL(_serverName, nick, channelName));
	else if (channelMode['i'] && !channel.isInvited(client))
		client.appendResponse(ERR_INVITEONLYCHAN(_serverName, nick, channelName));
	else
	{
		channel.setClientList(&client, false);
		std::string usersInChannel;
		channel.getUserListInChannel(usersInChannel);
		channel.relayMessage(client, JOIN(nick, client.getUsername(), client.getHostname(), channelName));
		client.setChannelJoined(&channel);
		client.appendResponse(RPL_TOPIC(_serverName, nick, channelName, channel.getTopic()));
		client.appendResponse(RPL_NAMREPLY(_serverName, nick, "=", channelName, usersInChannel));
		client.appendResponse(RPL_ENDOFNAMES(_serverName, nick, channelName));
	}
}

void Server::_joinNewChannel(Client &client, std::string const &channelName, std::string const &password)
{
	std::string const &nick = client.getNickname();
	Channel *newChannel = new Channel(channelName, client, _config);
	_channelList.push_back(newChannel);
	client.setChannelJoined(newChannel);
	newChannel->addClient(client, true);
	std::string usersInChannel;
	newChannel->getUserListInChannel(usersInChannel);
	client.appendResponse(RPL_NAMREPLY(_serverName, nick, "=", newChannel->getChannelName(), usersInChannel));
	client.appendResponse(RPL_ENDOFNAMES(_serverName, nick, newChannel->getChannelName()));
	newChannel->setChannelKey(password);
}

void Server::_createChannelKeyMap(std::vector<std::string> &params, std::map<std::string, std::string> &channelKeyMap)
{
	std::vector<std::string> channelList = _splitString(params[1], ',');
	std::vector<std::string> keyList;
	if (params.size() > 2)
		keyList = _splitString(params[2], ',');

	for (unsigned long i = 0; i < channelList.size(); i++)
	{
		if (keyList.size() > i)
			channelKeyMap.insert(std::pair<std::string, std::string>(channelList[i], keyList[i]));
		else
			channelKeyMap.insert(std::pair<std::string, std::string>(channelList[i], ""));
	}
}

void Server::_handleJoinCommand(Client &client, std::string &message)
{
	std::string const &nick = client.getNickname();
	std::vector<std::string> params = _splitString(message, ' ');
	if (params.size() < 2)
	{
		client.appendResponse(ERR_NEEDMOREPARAMS(_serverName, nick, "JOIN"));
		return;
	}

	std::map<std::string, std::string> channelKeyMap;
	_createChannelKeyMap(params, channelKeyMap);
	for (std::map<std::string, std::string>::iterator it = channelKeyMap.begin(); it != channelKeyMap.end(); ++it)
	{
		std::vector<Channel *>::iterator ite = std::find_if(_channelList.begin(), _channelList.end(), MatchChannelName(it->first));
		if (ite != _channelList.end())
			_joinExistingChannel(client, **ite, it->second);
		else
			_joinNewChannel(client, it->first, it->second);
	}
}
