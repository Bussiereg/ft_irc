/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverPolling.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 16:35:02 by mwallage          #+#    #+#             */
/*   Updated: 2024/05/18 17:26:49 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

void Server::startPolling()
{
	while (g_quit == false)
	{
		int numEvents = poll(_allSockets.data(), _allSockets.size(), -1);
		if (numEvents < 0)
			break;
		if (numEvents == 0)
			continue;
		if (_allSockets[0].revents & POLLIN)
			_acceptNewClient();
		_checkClients();
	}
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

	pollfd	newClientSocket;
	newClientSocket.fd = clientFd;
	newClientSocket.events = POLLIN | POLLOUT;
	newClientSocket.revents = 0;
	_allSockets.push_back(newClientSocket);

	Client* newClient = new Client(&_allSockets.back());
	_clients.push_back(newClient);
}

void Server::_checkClients()
{
	for (size_t i = _allSockets.size() - 1; i > 0; i--)
	{
		if (_allSockets[i].revents & POLLIN)
		{
			std::string buffer;
 			if (_fillBuffer(i, buffer) > 0) {
				_readBuffer(i, buffer);
			} else {
				_delClient(i);
			}
		}
	}
}

void Server::_delClient(size_t index)
{
	std::cerr << "[Server] Client fd " << _allSockets[index].fd << " just disconnected" << std::endl;
	_allSockets.erase(_allSockets.begin() + index);
	delete _clients[index - 1];
	_clients.erase(_clients.begin() + index - 1);
}

