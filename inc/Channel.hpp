#pragma once
#include <map>
#include <algorithm>
#include "Client.hpp"
#include "replies.hpp"
#include "Server.hpp"


class Channel
{
private:
	std::string _channelName;
	std::map<char, bool> _mode;
	std::string _topic;
	std::vector<Client*> _clientList;
	std::vector<Client*> _operator;
public:
	Channel(std::string & name, Client & client);
	~Channel();
	std::string	getTopic() const;
	void	setTopic(Client & lhs, std::string & newTopic);
	void	setChannelMode(char mode, bool status);
	std::string getChannelName() const;
};