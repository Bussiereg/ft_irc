#include "Server.hpp"

void Server::_joinExistingChannel(Client & client, Channel & channel, std::string const & key)
{
	std::string const & nick = client.getNickname();
	std::map<char, bool> & channelMode = channel.getChannelMode();
	std::string const & channelName = channel.getChannelName();
	std::vector<std::string> & inviteList = channel.getInviteList();
	
	if (channelMode['k'] && key != channel.getChannelKey())
		client.appendResponse(ERR_BADCHANNELKEY(_serverName, nick, channelName));
	else if (channelMode['l'] && (channel.getLimitUsers() <= channel.getClientList().size()))
		client.appendResponse(ERR_CHANNELISFULL(_serverName, nick, channelName));
	else if (channelMode['i']) {
		std::vector<std::string>::iterator it = std::find(inviteList.begin(), inviteList.end(), nick);
		if (it == inviteList.end())
		{
			client.appendResponse(ERR_INVITEONLYCHAN(_serverName, nick, channelName));
		}
	} else {
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

void Server::_joinNewChannel(Client & client, std::string const & channelName, std::string const & password)
{
	std::string const & nick = client.getNickname();
	Channel *newChannel = new Channel(channelName, client);
	_channelList.push_back(newChannel);	 // add the channel to the SERVER channel list
	client.setChannelJoined(newChannel); // add the channel to the CLIENT channel joined list
	newChannel->addClient(client, true); // add the client to the CHANNEL client list
	std::string usersInChannel;
	newChannel->getUserListInChannel(usersInChannel);
	client.appendResponse(RPL_NAMREPLY(_serverName, nick, "=", newChannel->getChannelName(), usersInChannel));
	client.appendResponse(RPL_ENDOFNAMES(_serverName, nick, newChannel->getChannelName()));
	newChannel->setChannelKey(password);
}

void Server::_handleJoinCommand(Client &client, std::string &message)
{
	std::map<std::string, std::string> joinParams;
	_createJoinmap(client, message, joinParams);
	for (std::map<std::string, std::string>::iterator it = joinParams.begin(); it != joinParams.end(); ++it)
	{
		std::vector<Channel *>::iterator ite = std::find_if(_channelList.begin(), _channelList.end(), MatchChannelName(it->first));
		if (ite != _channelList.end())
			_joinExistingChannel(client, **ite, it->second);
		else
			_joinNewChannel(client, it->first, it->second);
	}
}
