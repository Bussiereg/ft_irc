#pragma once
#include <map>
#include <algorithm>
#include "Client.hpp"
#include "replies.hpp"
#include "Server.hpp"


class Channel
{
private:
	std::string					_channelName;
	std::map<char, bool>		_mode;
	std::string					_topic;
	std::map<Client*, bool>		_clientList;
public:
	Channel(std::string & name, Client & client);
	~Channel();
	void						setTopic(Client & lhs, std::string & newTopic);
	void						setChannelMode(char mode, bool status);
	std::string					getTopic() const;
	std::string 				getChannelName() const;
	std::map<Client*, bool>		getClientList();
};