/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 16:43:26 by mwallage          #+#    #+#             */
/*   Updated: 2024/05/24 17:00:10 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

extern bool	g_quit;

class Client;
class Channel;

#define BUFFER_SIZE 512

class Server
{
private:
	typedef void (Server::*CommandFunction)(Client &, std::string &);
	std::map<std::string, CommandFunction> _commandMap;

	std::vector<Channel*>	_channelList;
	std::vector<Client*>	_clients;
	std::vector<pollfd>		_allSockets;

	std::string	const		_serverName;
	int						_port;
	std::string const		_password;

	void		_initCommandMap();
	void		_acceptNewClient();
	void		_delClient(Client & client);
	void		_checkClients();
	bool		_isNickInUse(std::string const & nick);

	// Join
	void 		_createJoinmap(Client & client, std::string & message, std::map<std::string, std::string> & joinParams);
	std::vector<Channel*>::iterator		isChannelAlreadyExisting(std::string rhs);


	// MODE
	unsigned int				_findMode(char m);
	void						modeInviteOnly(bool isOperator, Channel & channel);
	void						modeTopic(bool isOperator, Channel & channel);
	void						modeKeySet(bool isOperator, std::string key, Channel * channel);
	void						modeOperatorPriv(bool isOperator, std::string ope, Client & client, Channel * channel);
	void						modeSetUserLimit(bool isOperator, std::string limit, Channel & channel);

	void						_printBuffer(const char* buff, int recevied);
	ssize_t						_fillBuffer(size_t index, std::string & buffer);

	void 						_readBuffer(size_t index, std::string & buffer);
	std::string 				_getCommand(std::string &);
	void						_handlePassCommand(Client &, std::string &);
	void						_handleNickCommand(Client &, std::string &);
	void						_handleUserCommand(Client &, std::string &);
	void						_handlePrivmsgCommand(Client &, std::string &);
	void						_handleJoinCommand(Client &, std::string &);
	void						_handleTopicCommand(Client &, std::string &);
	void						_handleModeCommand(Client &, std::string &);
	void						_handlePingCommand(Client &, std::string &);
	void						_handleCapCommand(Client &, std::string &);
	void						_handleQuitCommand(Client &, std::string &);
	void						_handleInvalidCommand(Client &, std::string &);
	void						_handleWhoCommand(Client &, std::string &);

	// utils
	static std::vector<std::string>	_splitString(const std::string & str, char separator);
	std::string						_concatenateTokens(const std::vector<std::string>& tokens, size_t startPos);
	std::string						_getNextLine(std::string & buffer);
	static std::vector<std::string>	_parseReceivers(const std::string& message);

public:
	Server(int port, std::string password, std::string serverName);
	Server(Server const &);
	Server & operator=(Server const &);
	~Server();

	std::vector<Channel*> 	getChannelList();
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
