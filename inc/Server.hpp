/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 16:43:26 by mwallage          #+#    #+#             */
/*   Updated: 2024/05/15 16:02:39 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "Client.hpp"
#include <cstdlib>
#include <fcntl.h>

extern bool	g_quit;

class Server
{
private:
	std::vector<Client> _clients;
	pollfd				_serverSocket;
	std::vector<pollfd>	_allSockets;
	int					_port;
	std::string			_password;

	void _initAllSockets();
	void _updateAllSockets();
	void _pollSockets();
	void _acceptNewClient();
	void _listenToClients();
	void _handleNickCommand(Client& client, const std::string & nickname);
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
