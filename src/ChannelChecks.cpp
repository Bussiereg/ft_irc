#include "Channel.hpp"

bool Channel::isEmpty() const
{
	return _clientList.empty();
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