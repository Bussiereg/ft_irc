#include "Server.hpp"

/// @brief delete the channel if it is empty after a client leaves
/// @param channel channel to be deleted
void	Server::_delChannel(Channel * channel)
{	
	std::cout << "[Server  ] Channel " << channel->getChannelName() << " deleted" << std::endl;
	std::vector<Channel*>::iterator it = std::find(_channelList.begin(), _channelList.end(), channel);
	int channelIndex = std::distance(_channelList.begin(), it);
	_channelList.erase(_channelList.begin() + channelIndex);
	delete channel;
}

/// @brief delete client from all channels they are on
/// @param client 
void	Server::_delClientFromChannel(Client & client){
	std::vector<Channel*> & channels = client.getChannelJoined();
	for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		Channel& channel = **it;
		if (!channel.isMember(client))
			continue;

		channel.removeClient(&client);
		if (channel.isEmpty())
			_delChannel(&channel);
	}
}

/// @brief delete client from client vector, close the socket, and remove pollfd from _allSockets
/// @param client 
void	Server::_delClient(Client & client)
{
	std::vector<Client*>::iterator clientIt = std::find(_clients.begin(), _clients.end(), &client);
	int index = std::distance(_clients.begin(), clientIt);

	std::cerr << RED << "[Server] Client fd " << _allSockets[index + 1].fd << " just disconnected" << RESET << std::endl;

	close(_allSockets[index + 1].fd);
	_allSockets.erase(_allSockets.begin() + index + 1);
	delete _clients[index]; 
	_clients.erase(_clients.begin() + index);
}

/// @brief remove client from from all the channels they are on, and delete the client
/// @param client client to be removed
void Server::_removeCLient(Client & client)
{
	_delClientFromChannel(client);
	_delClient(client);
}