#pragma once
#include <map>
#include <algorithm>
#include "Client.hpp"
#include "replies.hpp"
#include "Server.hpp"

class Client;

class Channel
{
private:
	std::string					_channelName;
	std::string					_channelPassword;
	std::map<char, bool>		_mode;
	std::string					_topic;
	std::map<Client*, bool>		_clientList; // second param bool is true if the client if operator, false otherwise
public:
	Channel(std::string, Client &);
	~Channel();
	void						setTopic(Client & lhs, std::string & newTopic);
	void						setChannelMode(char mode, bool status);
	std::string					getTopic() const;
	std::string 				getChannelName() const;
	std::string 				getChannelPassword() const;
	std::map<Client*, bool>		getClientList();
};