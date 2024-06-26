#pragma once
#include <iostream>
#include <string>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include "Channel.hpp"

class Channel;

class Client {
private:
	pollfd *				_socket;
	std::string				_nickname;
	std::string				_username;
	std::string				_hostname;
	std::string				_realname;
	bool					_isPassedWord;
	bool					_isFullyAccepted;
	std::vector<Channel*>	_channelJoined;
	std::string				_response;
	std::string				_buffer;

public:
	Client(pollfd * socket);
	Client(Client const &);
	Client & operator=(Client const &);
	~Client();

	pollfd const * 			getClientSocket() const;
	std::string const &		getNickname() const;
	void					setNickname(std::string const &);
	std::string const & 	getUsername() const;
	void					setUsername(std::string const &);
	std::string const &		getHostname() const;
	void					setHostname(std::string const &);
	std::string const &		getRealname() const;
	void					setRealname(std::string const &);

	std::string const &		getResponse() const;
	void					appendResponse(std::string newMessage);
	void					clearResponse();

	std::string const &		getBuffer() const;
	void					appendBuffer(std::string const &);
	void					clearBuffer();
	std::string				PopNextLine();

	std::vector<Channel*> & getChannelJoined();
	void			 		removeChannelJoined(Channel * channel);
	void					setChannelJoined(Channel*);

	bool					isPassedWord() const;
	void					acceptPassword();
	bool 					isFullyAccepted() const;
	void					acceptFully();

};

class MatchNickname {
public:
	MatchNickname(const std::string& nickname) : nickname_(nickname) {}

	bool operator()(const Client* user) const {
		return user->getNickname() == nickname_;
	}

private:
	std::string nickname_;
};
