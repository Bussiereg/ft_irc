/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 16:49:06 by mwallage          #+#    #+#             */
/*   Updated: 2024/05/16 17:32:02 by mwallage         ###   ########.fr       */
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
	for (size_t i = 0; i < _allSockets.size(); i++) {
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
			break ;
		if (numEvents == 0)
			continue ;
		if (_allSockets[0].revents & POLLIN)
			_acceptNewClient();
		_listenToClients();
//		_updateAllSockets();
	}
}

void Server::_updateAllSockets()
{
	for (size_t i = 1; i < _allSockets.size(); i++)
	{
		if (_allSockets[i].fd == -1)
		{
			std::cout << "Removing client " << (_clients.begin() + i - 1)->getClientSocket().fd << std::endl;
			_allSockets.erase(_allSockets.begin() + i);
			_clients.erase(_clients.begin() + i - 1);
			i--;
		}
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
		return ;
	}

	std::cout << "Accepting new client " << clientFd << std::endl;
	fcntl(clientFd, F_SETFL, O_NONBLOCK);

	Client newClient(clientFd);
	_clients.push_back(newClient);
	_allSockets.push_back(newClient.getClientSocket());
}

void Server::_listenToClients()
{
	for (size_t i = 1; i < _allSockets.size(); i++)
	{
		if (_allSockets[i].revents & POLLIN)
		{
			char buffer[1024];
			memset(buffer, 0, 1024);
			int bytesRead = recv(_allSockets[i].fd, buffer, 1024, 0);

			if (bytesRead == -1)
			{
				std::cerr << "[Server] recv() failed on client fd " << _allSockets[i].fd << std::endl;
				_allSockets.erase(_allSockets.begin() + i);
				_clients.erase(_clients.begin() + i - 1);
				i--;
			}
			else if (bytesRead == 0)
			{
				std::cerr << "[Server] Client fd " << _allSockets[i].fd << " just disconnected" << std::endl;
				_allSockets.erase(_allSockets.begin() + i);
				_clients.erase(_clients.begin() + i - 1);
				i--;
			}
			else
			{
				std::cout << "[Client] Message received from client " << std::endl;
				std::cout << _allSockets[i].fd << " << " << buffer << std::endl;
			}
			
			std::cout << std::endl << "***list of clients***" << std::endl;
			for (size_t j = 0; j < _clients.size(); j++)
			{
				std::cout << "_clients[" << j << "].fd = " << _clients[j].getClientSocket().fd << std::endl;
			}
			std::cout << "***" << std::endl << std::endl;
		}
	}
}

void Server::_handleNickCommand(Client &client, const std::string &nickname)
{
	client.setNickname(nickname);

	std::string response = "Your nickname has been set to: " + client.getNickname() + "\r\n";
	std::cout << response;

	pollfd clientPoll = client.getClientSocket();
	send(clientPoll.fd, response.c_str(), response.size(), 0);
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
