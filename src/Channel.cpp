#include "Channel.hpp"

Channel::Channel(std::string name, Client & client) : _channelName(name){
	_clientList.insert(std::pair<Client*, bool>(&client, true));
	std::cout << YELLOW << "Constructor Channel called" << RESET << std::endl;	
	_mode['i'] = false;
	_mode['t'] = false;
	_mode['k'] = false;
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
	_ChannelKey = key;
}

void	Channel::setChannelLimit(unsigned int limit){
	_limitUsers = limit;
}

void	Channel::setInviteList(std::string nickname){
	_inviteList.push_back(nickname);
}

std::vector<std::string> &	Channel::getInviteList(){
	return _inviteList;
}

std::string	const & Channel::getTopic() const{
	return _topic;
}

void	Channel::setTopic(std::string & newTopic){
	_topic = newTopic;
}

std::map<Client*, bool> & Channel::getClientList(){
	return _clientList;
}

void	Channel::addClient(Client & client, bool isOperator){
	_clientList.insert(std::pair<Client*, bool>(&client, isOperator));
}

bool Channel::isModeOn(char mode) const
{
	if (_mode.find(mode) != _mode.end())
		return _mode.at(mode);
	return false;
}

bool Channel::isMember(Client & client){
	if (_clientList.find(&client) != _clientList.end())
		return true;
	return false;
}

bool Channel::isMember(std::string const & nickname){
	for (std::map<Client*, bool>::iterator it = _clientList.begin(); it != _clientList.end(); ++it){
		if (it->first->getNickname() == nickname)
			return true;
	}
	return false;
}

bool Channel::isOperator(Client & client)
{
	if (_clientList.find(&client) != _clientList.end())
		return _clientList[&client];
	return false;
}

bool Channel::isInvited(Client & client)
{
	return std::find(_inviteList.begin(), _inviteList.end(), client.getNickname()) != _inviteList.end();
}

std::string const &	Channel::getChannelName() const{
	return _channelName;
}

std::string const &	Channel::getChannelKey() const{
	return _ChannelKey;
}

std::map<char, bool> &		Channel::getChannelMode(){
	return _mode;
}

void	Channel::setClientList(Client * client, bool isOperator){
	bool isClientInList = _clientList.find(client) != _clientList.end(); 
	if (isClientInList)
		 _clientList[client] = isOperator;
	else{
		_clientList.insert(std::pair<Client*, bool>(client, isOperator));
	}
}

unsigned int				Channel::getLimitUsers(){
	return _limitUsers;
}

std::string		Channel::getModeString(){
	std::string modeString = "";
	std::map<char, bool>::iterator it;
	for (it = _mode.begin(); it != _mode.end(); ++it){
		if (it->second == true)
			modeString += it->first;
	}
	return modeString;
}

void	Channel::removeClient(Client * client){
	_clientList.erase(client);
}

Channel::~Channel(){
}