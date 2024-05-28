#pragma once
#include <map>
#include <algorithm>
#include "Server.hpp"
#include "Client.hpp"
#include "replies.hpp"

class Client;

class Channel
{
private:
	std::string					_channelName;
	std::string					_channelPassword;
	std::map<char, bool>		_mode;
	std::string					_topic;
	unsigned int				_limitUsers;
	std::map<Client*, bool>		_clientList; // second param bool is true if the client if operator, false otherwise
public:
	Channel(std::string, Client &);
	~Channel();

	void						setTopic(Client & lhs, std::string & newTopic);
	void						setChannelMode(char mode, bool status);
	void						setChannelKey(std::string key);
	void						setChannelLimit(unsigned int limit);
	unsigned int				getLimitUsers();
	std::string					getModeString();
	std::string	const &			getTopic() const;
	std::string const & 		getChannelName() const;
	std::string const &			getChannelPassword() const;
	std::map<Client*, bool> &	getClientList();
	void						setClientList(Client *, bool);
	void						addClient(Client & client, bool isOperator);
	bool						isMember(Client & client);
	void						relayMessage(Client & sender, std::string const & message);

	void 						getUserListInChannel(std::string & usersInChannel);
};

class MatchChannelName {
public:
    MatchChannelName(const std::string& name) : name_(name) {}

    bool operator()(Channel* const channel) const {
        return channel->getChannelName() == name_;
    }

private:
    std::string name_;
};
