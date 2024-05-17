/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 16:49:06 by mwallage          #+#    #+#             */
/*   Updated: 2024/05/17 16:37:25 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

Server::Server(int port, std::string password) : _port(port), _password(password)
{
	_serverSocket.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket.fd == -1)
		throw SocketCreationException();
	fcntl(_serverSocket.fd, F_SETFL, O_NONBLOCK);

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	if (_port < 1500)
		throw std::runtime_error("invalid port number");
	serverAddress.sin_port = htons(_port);

	if (bind(_serverSocket.fd, reinterpret_cast<sockaddr *>(&serverAddress),
			 sizeof(serverAddress)) == -1)
	{
		close(_serverSocket.fd);
		throw SocketBindingException();
	}

	if (listen(_serverSocket.fd, 10) == -1)
	{
		close(_serverSocket.fd);
		throw SocketListeningException();
	}

	std::cout << "Server listening on port " << port << std::endl;
}

Server::Server(Server const &other)
	: _clients(other._clients), _serverSocket(other._serverSocket)
{
}

Server &Server::operator=(Server const &other)
{
	if (this != &other)
	{
		_clients = other._clients;
		_serverSocket = other._serverSocket;
	}
	return *this;
}

Server::~Server()
{
	std::cout << "Closing server..." << std::endl;
	for (size_t i = 0; i < _allSockets.size(); i++)
	{
		close(_allSockets[i].fd);
	}
	_clients.erase(_clients.begin(), _clients.end());
}

void Server::startPolling()
{
	_initAllSockets();
	while (g_quit == false)
	{
		int numEvents = _pollSockets();
		if (numEvents < 0)
			break;
		if (numEvents == 0)
			continue;
		if (_allSockets[0].revents & POLLIN)
			_acceptNewClient();
		_checkClients();
	}
}

void Server::_initAllSockets()
{
	_serverSocket.events = POLLIN;
	_serverSocket.revents = 0;
	_allSockets.push_back(_serverSocket);
}

int Server::_pollSockets()
{
	return poll(_allSockets.data(), _allSockets.size(), -1);
}

void Server::_acceptNewClient()
{
	sockaddr_in clientAddress;
	socklen_t clientAddressSize = sizeof(clientAddress);
	int clientFd = accept(_allSockets[0].fd, reinterpret_cast<sockaddr *>(&clientAddress), &clientAddressSize);
	if (clientFd == -1)
	{
		std::cerr << "failed to accept new connection" << std::endl;
		return;
	}

	std::cout << "Accepting new client " << clientFd << std::endl;
	fcntl(clientFd, F_SETFL, O_NONBLOCK);

	Client newClient(clientFd);
	_clients.push_back(newClient);

	pollfd newClientPoll;
	newClientPoll.fd = clientFd;
	newClientPoll.events = POLLIN | POLLOUT;
	newClientPoll.revents = 0;
	_allSockets.push_back(newClientPoll);
}

void Server::_checkClients()
{
	for (size_t i = 1; i < _allSockets.size(); i++)
	{
		if (_allSockets[i].revents & POLLIN)
		{
			_readClient(i);
		}
	}
}

int Server::_delClient(size_t index)
{
	_allSockets.erase(_allSockets.begin() + index);
	_clients.erase(_clients.begin() + index - 1);
	return -1;
}

void Server::_readClient(size_t & index)
{
	std::string buffer;
 	if (_fillBuffer(index, buffer) <= 0) {
		std::cerr << "[Server] Client fd " << _allSockets[index].fd << " just disconnected" << std::endl;
		index += _delClient(index);
	}

	Client &client = _clients[index - 1];
	std::string response;
	std::string message;

	while (!(message = _getNextLine(index, buffer)).empty())
	{
		std::cout << "[Client] Message received from client " << std::endl;
		std::cout << _allSockets[index].fd << " << " << message << std::endl;

		enum Commands commandCase = _getCommand(message);
		switch (commandCase) {
			case PASS:
				response = _handlePassCommand(client, message);
				break;
			case NICK:
				response = _handleNickCommand(client, message);
				break;
			case USER:
				response = _handleUserCommand(client, message);
				break;
			case PRIVMSG:
				response = _handlePrivmsgCommand(client, message);
				break;
			case INVALID:
				response = client.getNickname() + ' ' + message + " :Unknown command\r\n";
		}

		if (!response.empty() && _allSockets[index].revents & POLLOUT) {
			send(_allSockets[index].fd, response.c_str(), response.size(), 0);
		}
	}

	std::cout << std::endl << "***list of clients***" << std::endl;
	for (size_t j = 0; j < _clients.size(); j++)
	{
		std::cout << "_clients[" << j << "].fd = " << _clients[j].getClientFd() << std::endl;
	}
	std::cout << "***" << std::endl << std::endl;
}

const char *Server::SocketCreationException::what() const throw()
{
	return "ircserv: error creating socket";
}

const char *Server::SocketBindingException::what() const throw()
{
	return "ircserv: error binding socket";
}

const char *Server::SocketListeningException::what() const throw()
{
	return "ircserv: error listening on socket";
}
