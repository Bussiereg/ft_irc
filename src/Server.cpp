/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 16:31:12 by mwallage          #+#    #+#             */
/*   Updated: 2024/05/23 19:32:26 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int port, std::string password, std::string serverName) : _serverName(serverName), _port(port), _password(password)
{
	pollfd	serverSocket;
	serverSocket.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket.fd == -1)
		throw SocketCreationException();
	fcntl(serverSocket.fd, F_SETFL, O_NONBLOCK);

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	if (_port < 1500)
		throw std::runtime_error("invalid port number");
	serverAddress.sin_port = htons(_port);

	if (bind(serverSocket.fd, reinterpret_cast<sockaddr *>(&serverAddress),
			 sizeof(serverAddress)) == -1)
	{
		close(serverSocket.fd);
		throw SocketBindingException();
	}

	if (listen(serverSocket.fd, 10) == -1)
	{
		close(serverSocket.fd);
		throw SocketListeningException();
	}

	_initCommandMap();

	serverSocket.events = POLLIN;
	serverSocket.revents = 0;
	_allSockets.push_back(serverSocket);
	std::cout << YELLOW << "Server listening on port " << port << "\n" << RESET << std::endl;
}

void Server::_initCommandMap()
{
	_commandMap["PASS"] = &Server::_handlePassCommand;
	_commandMap["NICK"] = &Server::_handleNickCommand;
	_commandMap["USER"] = &Server::_handleUserCommand;
	_commandMap["PRIVMSG"] = &Server::_handlePrivmsgCommand;
	_commandMap["PING"] = &Server::_handlePingCommand;
	_commandMap["JOIN"] = &Server::_handleJoinCommand;
	_commandMap["QUIT"] = &Server::_handleQuitCommand;
	_commandMap["CAP"] = &Server::_handleCapCommand;
	_commandMap["INVALID"] = &Server::_handleInvalidCommand;
}

Server::Server(Server const &other)
	: _clients(other._clients), _allSockets(other._allSockets), _serverName(other._serverName), _port(other._port), _password(other._password)
{
}

Server &Server::operator=(Server const &other)
{
	if (this != &other)
	{
		_clients = other._clients;
		_allSockets = other._allSockets;
		_port = other._port;
	}
	return *this;
}

Server::~Server()
{
	std::cout << "Closing server..." << std::endl;
	close(_allSockets[0].fd);
	for (size_t i = 1; i < _allSockets.size(); i++)
	{
		close(_allSockets[i].fd);
		delete _clients[i - 1];
	}
	_clients.clear();
}
