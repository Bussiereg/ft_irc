#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <poll.h>
#include <iomanip>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdlib>
#include <fcntl.h>
#include <cstring>
#include <map>
#include <set>
#include <algorithm>
#include <sstream>
#include <utility>
#include "Client.hpp"
#include "Channel.hpp"
#include "replies.hpp"
#include "ColorPrint.hpp"
#include "ConfigParser.hpp"

extern bool	g_quit;

class Client;
class Channel;

enum mode {
	INVITE_ONLY,
	TOPIC,
	KEY,
	OPERATOR_PRIVILEGE,
	LIMIT
};

#define BUFFER_SIZE 512

#ifndef HOSTNAME
# define HOSTNAME "localhost"
#endif

class Server
{
private:
	typedef void (Server::*CommandFunction)(Client &, std::string &);
	std::map<std::string, CommandFunction> _commandMap;

	std::vector<Channel*>	_channelList;
	std::vector<Client*>	_clients;
	std::vector<pollfd>		_allSockets;

	ConfigParser			_config;
	std::string	const		_serverName;
	int const				_port;
	std::string const		_password;

	void					_initCommandMap();
	void					_acceptNewClient();
	void					_delClientAndChannel(Client &);
	void					_checkClients();
	void					_registerUser(Client &);
	Client *				_findClient(std::string const & nickname);
	Client *				_findClient(int fd);

	// JOIN
	void					_createChannelKeyMap(std::vector<std::string> & params, std::map<std::string, std::string> & channelKeyMap);
	void					_joinExistingChannel(Client & client, Channel & channel, std::string const & password);
	void					_joinNewChannel(Client & client, std::string const & channelName, std::string const & password);

	// MODE
	void					_modeInviteOnly(bool isOperator, Channel &);
	void					_modeTopic(bool isOperator, Channel &);
	void					_modeKeySet(bool isOperator, std::string key, Channel *);
	void					_modeOperatorPriv(bool isOperator, std::string ope, Client &, Channel *);
	void					_modeSetUserLimit(bool isOperator, std::string limit, Channel &);

	void					_printBuffer(const char* buff, int recevied);
	ssize_t					_fillBuffer(size_t index, std::string & buffer);

	void 					_readBuffer(size_t index, std::string & buffer);
	std::string 			_getCommand(std::string &);
	void					_handlePassCommand(Client &, std::string &);
	void					_handleNickCommand(Client &, std::string &);
	void					_handleUserCommand(Client &, std::string &);
	void					_handlePrivmsgCommand(Client &, std::string &);
	void					_handleJoinCommand(Client &, std::string &);
	void					_handleTopicCommand(Client &, std::string &);
	void					_handleModeCommand(Client &, std::string &);
	void					_handlePingCommand(Client &, std::string &);
	void					_handleCapCommand(Client &, std::string &);
	void					_handleQuitCommand(Client &, std::string &);
	void					_handleInvalidCommand(Client &, std::string &);
	void					_handleWhoCommand(Client &, std::string &);
	void					_handleInviteCommand(Client &, std::string &);
	void					_handleKickCommand(Client &, std::string &);
	void					_handlePartCommand(Client &, std::string &);
	void					_handleMotdCommand(Client &, std::string &);

	// utils
	bool					_isNickInUse(std::string const & nick);
	bool					_isValidNickname(const std::string &nickname);
	static std::vector<std::string>	_splitString(const std::string & str, char separator);
	std::string				_concatenateTokens(const std::vector<std::string>& tokens, size_t startPos);
	std::string				_getNextLine(std::string & buffer);
	static std::vector<std::string>	_parseReceivers(const std::string& message);

public:
	Server(int port, std::string password, ConfigParser const &);
	Server(Server const &);
	Server & operator=(Server const &);
	~Server();

	void 					startPolling();
	
	class SocketCreationException : public std::exception {
		virtual const char *what() const throw();
	};

	class SocketBindingException : public std::exception {
		virtual const char* what() const throw();
	};

	class SocketListeningException : public std::exception {
		virtual const char* what() const throw();
	};
};
