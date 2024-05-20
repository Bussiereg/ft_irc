#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdlib>
#include <fcntl.h>
#include <cstring>
#include <algorithm>
#include <utility>
#include "Client.hpp"
#include "replies.hpp"

class Channel
{
private:
	std::string _channelName;
public:
	Channel(std::string name);
	~Channel();
};

Channel::Channel(std::string name) : _channelName(name){
}

Channel::~Channel()
{
}
