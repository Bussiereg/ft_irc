#include "Channel.hpp"

Channel::Channel(std::string & name, Client & client) : _channelName(name){
	_operator.push_back(&client);
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
	std::vector<Client*>::iterator it = _operator.begin();
	std::vector<Client*>::iterator ite = _operator.end();
	if (_mode['o'] && std::find(it, ite, lhs) != ite)
		_topic  = newTopic;
}


Channel::~Channel(){
}


std::string 	Channel::getChannelName() const{
	return _channelName;
}