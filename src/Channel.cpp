#include "Channel.hpp"

Channel::Channel(std::string name, Client & client) : _channelName(name){
	_clientList.insert(std::pair<Client*, bool>(&client, true));
	_topic = "this is the default Topics of the channel: " + _channelName;
	_mode['i'] = false;
	_mode['t'] = false;
	_mode['k'] = false;
	_mode['o'] = false;
	_mode['l'] = false;
}

/// @brief relay message to all clients in channel, except to the sender
/// @param sender  the user who is sending the message
/// @param message the message that needs to be relayed
void Channel::relayMessage(Client & sender, std::string const & message)
{
 	for (std::map<Client*, bool>::iterator it = _clientList.begin(); it != _clientList.end(); ++it)
	{
		std::cout << "Relaying message to " << it->first->getNickname() << std::endl;
		if ((it->first) != &sender)
			it->first->appendResponse(message);
	}
}

void	Channel::setChannelMode(char mode, bool status){
	if (_mode.find(mode) != _mode.end()) {
		_mode[mode] = status;
	}
}

void	Channel::setChannelKey(std::string key){
	_channelPassword = key;
}

void	Channel::setChannelLimit(unsigned int limit){
	_limitUsers = limit;
}

std::string	const & Channel::getTopic() const{
	return _topic;
}

void	Channel::setTopic(Client & lhs, std::string & newTopic){
	if (!_mode['t'])
		_topic  = newTopic;
	else if (_mode['t'] && _clientList[&lhs])
		_topic  = newTopic;
}

std::map<Client*, bool> & Channel::getClientList(){
	return _clientList;
}

void	Channel::addClient(Client & client, bool isOperator){
	_clientList.insert(std::pair<Client*, bool>(&client, isOperator));
}

bool Channel::isMember(Client & client){
	if (_clientList.find(&client) != _clientList.end())
		return true;
	return false;
}

std::string const &	Channel::getChannelName() const{
	return _channelName;
}

std::string const &	Channel::getChannelPassword() const{
	return _channelPassword;
}

void	Channel::setClientList(Client & client, bool isOperator){
	_clientList.insert(std::pair<Client*, bool>(&client, isOperator));
}

Channel::~Channel(){
}