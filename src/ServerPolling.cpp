#include "Server.hpp"

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
		std::cerr << RED << "failed to accept new connection" << RESET << std::endl;
		return;
	}

	std::cout << GREEN <<"Accepting new client fd " << clientFd << RESET << std::endl;
	fcntl(clientFd, F_SETFL, O_NONBLOCK);

	pollfd* clientSocket = new pollfd;
	clientSocket->fd = clientFd;
	clientSocket->events = POLLIN | POLLOUT;
	clientSocket->revents = 0;
	_allSockets.push_back(*clientSocket);

	Client* newClient = new Client(clientSocket);
	_clients.push_back(newClient);
}

void Server::_checkClients()
{
	for (size_t i = _allSockets.size() - 1; i > 0; i--)
	{
		Client & client = *_clients[i - 1];
		std::string const & response = client.getResponse();
		if (!response.empty() && _allSockets[i].revents & POLLOUT)
		{
			_printBuffer(response.c_str(), 0);
			ssize_t send_rt = send(_allSockets[i].fd, response.c_str(), response.size(), 0);
			std::cout << "send return = " << std::dec << send_rt << " - ";
			if (send_rt > -1)
				std::cout << "sent succesfully " << std::endl;
			else
				std::cout << "send error" << std::endl; // should throw an exception
			client.clearResponse();
		}
		if (_allSockets[i].revents & POLLIN)
		{
			std::string buffer;
 			if (_fillBuffer(client) <= 0) {
				_removeCLient(client);
			} else {
				_readBuffer(client);
			}
		}
	}
}

