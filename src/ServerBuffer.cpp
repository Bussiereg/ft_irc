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
			if (command == "QUIT")
				break;
		}
		else
		{
			client.appendResponse(ERR_NOTREGISTERED(_serverName, client.getNickname()));
		}
	}
}

ssize_t Server::_fillBuffer(Client &client)
{
	char temp[BUFFER_SIZE];

	memset(temp, 0, BUFFER_SIZE);
	ssize_t bytesRead = recv(client.getClientSocket()->fd, temp, BUFFER_SIZE - 1, 0);
	if (bytesRead > 0)
		client.appendBuffer(temp);
	return bytesRead;
}
