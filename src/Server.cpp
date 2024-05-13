/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 16:49:06 by mwallage          #+#    #+#             */
/*   Updated: 2024/05/13 15:52:05 by mwallage         ###   ########.fr       */
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
	closeServer();
}

void Server::startPolling()
{
	std::vector<pollfd> allSockets;

	_serverSocket.events = POLLIN;
	allSockets.push_back(_serverSocket);

	while (true)
	{
		std::vector<pollfd> newSockets(allSockets);

		int numEvents = poll(allSockets.data(), allSockets.size(), -1);
		if (numEvents == -1)
		{
			std::cerr << "poll() error\n";
			break ;
		}

		// Check for events on each file descriptor
		for (size_t i = 0; i < allSockets.size(); i++)
		{
			if (allSockets[i].revents & POLLIN)
			{
				if (allSockets[i].fd == _serverSocket.fd)
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
					ssize_t bytesRead = recv(allSockets[i].fd, buffer, sizeof(buffer), 0);
					if (bytesRead <= 0)
					{
						std::cout << "Socket " << allSockets[i].fd << " is empty" << std::endl;
						close(allSockets[i].fd);
						std::cout << "Should be the same as " << newSockets[i].fd << "?" << std::endl;
						newSockets.erase(newSockets.begin() + i);
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
		allSockets = newSockets;
	}
}

void Server::closeServer()
{
	close(_serverSocket.fd);

	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		std::cout << "Closing " << it->getClientPoll().fd << std::endl;
		close(it->getClientPoll().fd);
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
