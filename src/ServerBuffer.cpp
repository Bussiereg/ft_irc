#include "Server.hpp"

void Server::_readBuffer(Client &client)
{
	std::string message;

	while (!(message = client.PopNextLine()).empty())
	{
		std::string command = _getCommand(message);
		if (client.isFullyAccepted() || command == "LS" || command == "PASS" || (client.isPassedWord() && (command == "NICK" || command == "USER")))
		{
			(this->*_commandMap[command])(client, message);
		}
		else
		{
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

void Server::_printBuffer(const char *buff, int received)
{
	if (received)
		std::cout << "[buff  in] " << MAGENTA;
	else
		std::cout << "[buff out] " << YELLOW;
	for (int j = 0; buff[j] != 0; ++j)
	{
		if (std::isprint(buff[j]))
		{
			std::cout << buff[j];
		}
		else
		{
			std::cout << "\\x" << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)buff[j];
		}
	}
	std::cout << RESET << std::endl;
}

ssize_t Server::_fillBuffer(Client &client)
{
	char temp[BUFFER_SIZE];

	memset(temp, 0, BUFFER_SIZE);
	ssize_t bytesRead = recv(client.getClientSocket()->fd, temp, BUFFER_SIZE - 1, 0);
	_printBuffer(temp, 1);
	if (bytesRead > 0)
		client.appendBuffer(temp);
	return bytesRead;
}
