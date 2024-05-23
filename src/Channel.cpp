#include "Channel.hpp"

Channel::Channel(std::string name, Client & client) : _channelName(name){
	_clientList.insert(std::pair<Client*, bool>(&client, true));
	_topic = "example of topics";
	_mode['i'] = false;
	_mode['t'] = false;
	_mode['k'] = false;
	_mode['o'] = false;
	_mode['l'] = false;
}


void	Channel::setChannelMode(char mode, bool status){
	if (_mode.find(mode) != _mode.end()) {
		_mode[mode] = status;
	}
}

std::string	Channel::getTopic() const{
	return _topic;
}

void	Channel::setTopic(Client & lhs, std::string & newTopic){
	if (!_mode['t'])
		_topic  = newTopic;
	else if (_mode['t'] && _clientList[&lhs])
		_topic  = newTopic;
}

std::map<Client*, bool> Channel::getClientList(){
	return _clientList;
}

std::string 	Channel::getChannelName() const{
	return _channelName;
}

std::string 	Channel::getChannelPassword() const{
	return _channelPassword;
}

Channel::~Channel(){
}