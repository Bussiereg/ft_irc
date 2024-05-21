/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 16:43:26 by mwallage          #+#    #+#             */
/*   Updated: 2024/05/18 18:09:11 by mwallage         ###   ########.fr       */
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
#include <algorithm>
#include <utility>
#include "Client.hpp"
#include "replies.hpp"
#include "ColorPrint.hpp"

extern bool	g_quit;

#define BUFFER_SIZE 1024

enum Commands {
	PASS,
	NICK,
	USER,
	PRIVMSG,
	PING,
	INVALID
};

class Server
{
private:
	std::vector<Client*> _clients;
	std::vector<pollfd> _allSockets;
	int					_port;
	std::string			_password;

	void 	_acceptNewClient();
	void	_delClient(size_t index);
	void 	_checkClients();
	void 	_readBuffer(size_t index, std::string & buffer);
	void	_handleNickCommand(Client& client, const std::string & nickname);
	bool	_isNickInUse(std::string const & nick);
	void	_printBuffer(char* buff);

	ssize_t		_fillBuffer(size_t index, std::string & buffer);
	std::string _getNextLine(size_t & index, std::string & buffer);

	Commands 	_getCommand(std::string &);
	void		_handlePassCommand(Client &, std::string &);
	void		_handleNickCommand(Client &, std::string &);
	void		_handleUserCommand(Client &, std::string &);
	void		_handlePrivmsgCommand(Client &, std::string &);
public:
	Server(int port, std::string password);
	Server(Server const &);
	Server & operator=(Server const &);
	~Server();
	
	void startPolling();
	
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
