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
	if (_maxUsers > 0 && _clients.size() >= _maxUsers)
	{
		std::string const response = "ERROR: Server is full\r\n";
		send(clientFd, response.c_str(), response.size(), 0);
		close(clientFd);
		std::cout << RED << "Max users reached" << RESET << std::endl;
		return;
	}
	std::cout << GREEN << "Accepting new client fd " << clientFd << RESET << std::endl;
	fcntl(clientFd, F_SETFL, O_NONBLOCK);

	pollfd *clientSocket = new pollfd;
	clientSocket->fd = clientFd;
	clientSocket->events = POLLIN | POLLOUT;
	clientSocket->revents = 0;
	_allSockets.push_back(*clientSocket);

	Client *newClient = new Client(clientSocket);
	_clients.push_back(newClient);
}

void Server::_sendServerResponse(Client &client, std::string const &response)
{
	ssize_t bytes_sent = send(client.getClientSocket()->fd, response.c_str(), response.size(), 0);
	client.clearResponse();
	if (bytes_sent < 0)
		_removeClient(client);
}

void Server::_readClient(Client &client)
{
	if (_fillBuffer(client) <= 0)
		_removeClient(client);
	else
		_readBuffer(client);
}

void Server::_checkClients()
{
	for (size_t i = _allSockets.size() - 1; i > 0; i--)
	{
		Client &client = *_clients[i - 1];
		std::string const &response = client.getResponse();
		if (!response.empty() && _allSockets[i].revents & POLLOUT)
			_sendServerResponse(client, response);
		if (_allSockets[i].revents & POLLIN)
			_readClient(client);
	}
}
