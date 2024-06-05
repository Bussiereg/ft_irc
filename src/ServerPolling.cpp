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
		std::string const & response = _clients[i - 1]->getResponse();
		if (!response.empty() && _allSockets[i].revents & POLLOUT)
		{
			_printBuffer(response.c_str(), 0);
			ssize_t send_rt = send(_allSockets[i].fd, response.c_str(), response.size(), 0);
			std::cout << "send return = " << std::dec << send_rt << " - ";
			if (send_rt > -1)
				std::cout << "sent succesfully " << std::endl;
			else
				std::cout << "send error" << std::endl; // should throw an exception
			_clients[i - 1]->clearResponse();
		}
		if (_allSockets[i].revents & POLLIN)
		{
			std::string buffer;
 			if (_fillBuffer(i, buffer) <= 0) {
				_removeCLient(*_clients[i - 1]);
			} else {
				_readBuffer(i, buffer);
			}
		}
	}
}

void Server::_readBuffer(size_t index, std::string &buffer)
{
	Client &client = *_clients[index - 1];
	std::string message;

	while (!(message = _getNextLine(buffer)).empty())
	{
		std::cout << "[Client " << index << "] Message received from client " << std::endl;
		std::cout << "     FD " << _allSockets[index].fd << "< " << CYAN << message << RESET << std::endl;

		std::string command = _getCommand(message);
		if (client.isFullyAccepted()
			|| command == "LS"
			|| command == "PASS"
			|| (client.isPassedWord()
				&& (command == "NICK" || command == "USER"))) {
			(this->*_commandMap[command])(client, message);
		} else {
			client.appendResponse(ERR_NOTREGISTERED(_serverName, client.getNickname()));
		}
	}

	std::cout << std::endl
			  << "***list of clients***\n*" << std::endl;
	for (size_t j = 0; j < _clients.size(); j++)
	{
		std::cout << "* _clients[" << j << "].fd = " << _clients[j]->getClientSocket()->fd << "  ";
		std::cout << "_allSockets[" << j + 1 << "].fd = " << _allSockets[j + 1].fd << std::endl;
		;
	}
	std::cout << "*\n*********************" << std::endl
			  << std::endl;
}
