/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 16:49:06 by mwallage          #+#    #+#             */
/*   Updated: 2024/05/13 15:27:30 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

Server::Server()
{
	_serverSocket.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket.fd == -1)
		throw SocketCreationException();

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	serverAddress.sin_port = htons(6667);

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

	std::cout << "Server listening on port 6667..." << std::endl;
}

Server::Server(Server const &other)
	: _clients(other._clients)
{
}

Server &Server::operator=(Server const &other)
{
	if (this != &other)
	{
		_clients = other._clients;
		_serverSocket.fd = other._serverSocket.fd;
	}
	return *this;
}

Server::~Server()
{
	closeServer();
}

void Server::startPolling()
{
	_serverSocket.events = POLLIN;
	_allSockets.push_back(_serverSocket);

	while (true)
	{
		std::vector<pollfd> newSockets;
		int numEvents = poll(_allSockets.data(), _allSockets.size(), -1);
		if (numEvents == -1)
		{
			std::cerr << "poll() error\n";
			break ;
		}

		// Check for events on each file descriptor
		for (size_t i = 0; i < _allSockets.size(); i++)
		{
			if (_allSockets[i].revents & POLLIN)
			{
				if (_allSockets[i].fd == _serverSocket.fd)
				{
					// Accept incoming connections and handle each client separately
					sockaddr_in clientAddress;
					socklen_t clientAddressLength = sizeof(clientAddress);
					int clientSocket = accept(_serverSocket.fd,
											  reinterpret_cast<sockaddr *>(&clientAddress), &clientAddressLength);
					// accept() returns when the client calls connect() successfully
					if (clientSocket == -1)
					{
						std::cerr << "Error accepting connection." << std::endl;
						continue;
					}
					std::cout << "Client connected." << std::endl;
					Client newClient(clientSocket);
					_clients.push_back(newClient);

					// add to current loop
					pollfd newClientPoll;
					newClientPoll.fd = clientSocket;
					newClientPoll.events = POLLIN | POLLOUT;
					newSockets.push_back(newClientPoll);
				}
				else
				{
					// Receive and process incoming messages from client
					char buffer[1024];
					ssize_t bytesRead = recv(_allSockets[i].fd, buffer, sizeof(buffer), 0);
					if (bytesRead <= 0)
					{
						close(_allSockets[i].fd);
						_allSockets.erase(_allSockets.begin() + i);
						continue;
					}

					// parse the received message
					std::string message(buffer, bytesRead);

					if (message.substr(0, 5) == "NICK ")
					{
						std::string nickname = message.substr(5);
						_handleNickCommand(_clients.back(), nickname);
					}
				}
			}
			// Add handling for other events (e.g., POLLHUP, POLLERR)
		}
		_allSockets.insert(_allSockets.end(), newSockets.begin(), newSockets.end());
	}
}

void Server::closeServer()
{
	for (std::vector<pollfd>::iterator it = _allSockets.begin(); it != _allSockets.end(); ++it)
	{
		std::cout << "Closing " << it->fd << std::endl;
		close(it->fd);
	}
}

void Server::_handleNickCommand(Client &client, const std::string &nickname)
{
	client.setNickname(nickname);

	std::string response = "Your nickname has been set to: " + client.getNickname() + "\r\n";
	std::cout << response;

	pollfd clientPoll = client.getClientPoll();
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
