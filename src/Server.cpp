/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 16:49:06 by mwallage          #+#    #+#             */
/*   Updated: 2024/05/14 16:04:32 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

Server::Server(int port, std::string password) : _port(port), _password(password)
{
	_serverSocket.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket.fd == -1)
		throw SocketCreationException();

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
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
	closeServer();
}

void Server::startPolling()
{
	std::vector<pollfd> allSockets;

	_serverSocket.events = POLLIN;
	allSockets.push_back(_serverSocket);

	while (true)
	{
		size_t size = allSockets.size();
		int numEvents = poll(allSockets.data(), size, -1);
		if (numEvents == -1)
			break ;


		// Check for events on each file descriptor
		for (size_t i = 1; i < size; i++)
		{
			if (allSockets[i].fd != -1 && allSockets[i].revents & POLLIN)
			{
				// Receive and process incoming messages from client
				char buffer[1024];
				ssize_t bytesRead = recv(allSockets[i].fd, buffer, sizeof(buffer), 0);
				if (bytesRead <= 0)
				{
					std::cout << "Socket " << allSockets[i].fd << " is empty/disconnected" << std::endl;
					close(allSockets[i].fd);
					allSockets.erase(allSockets.begin() + i);
					i--;
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
		
		if (allSockets[0].revents & POLLIN)
		{
			// Accept incoming connections and handle each client separately
			sockaddr_in clientAddress;
			socklen_t clientAddressLength = sizeof(clientAddress);
			int clientSocket = accept(_serverSocket.fd, reinterpret_cast<sockaddr *>(&clientAddress), &clientAddressLength);
			if (clientSocket == -1)
			{
				std::cerr << "Error accepting connection." << std::endl;
				continue ;
			}
			std::cout << "Client connected." << std::endl;
			Client newClient(clientSocket);
			_clients.push_back(newClient);

			pollfd newClientPoll;
			newClientPoll.fd = clientSocket;
			newClientPoll.events = POLLIN | POLLOUT;
			newClient.setClientPoll(newClientPoll);
			allSockets.push_back(newClientPoll);
		}
	}
}

void Server::closeServer()
{
	close(_serverSocket.fd);
}

int Server::_acceptClient()
{

	return 0;
}

int Server::_handleCommand()
{
	return 0;
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
