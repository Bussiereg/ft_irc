#include "Server.hpp"

bool	Server::_isNickInUse(std::string const & nick)
{
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if ((*it)->getNickname() == nick) {
			return true;
		}
	}
	return false;
}

void Server::_printBuffer(const char* buff, int received)
{
	if (received)
		std::cout << "[buff  in] " << MAGENTA;
	else
		std::cout << "[buff out] " << YELLOW;
	for (int j = 0; buff[j] != 0; ++j) {
		if (std::isprint(buff[j])) {
			std::cout << buff[j];
		} else {
			std::cout << "\\x" << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)buff[j];
		}
	}
	std::cout << RESET << std::endl;
	//std::cout << "\n|----- end --------|\n\n" << std::endl;
	//std::cout << "|-- buffer ascii :|\n" << buff << "|----- end --------|\n" <<std::endl;
}

ssize_t	Server::_fillBuffer(size_t index, std::string & buffer)
{
	char temp[BUFFER_SIZE];

	memset(temp, 0, BUFFER_SIZE);
	ssize_t bytesRead = recv(_allSockets[index].fd, temp, BUFFER_SIZE - 1, 0);
	_printBuffer(temp, 1);
	if (bytesRead > 0)
		buffer.append(temp);
	return bytesRead;
}

std::string Server::_getNextLine(std::string & buffer)
{
	size_t pos;
	if ((pos = buffer.find("\r\n")) == std::string::npos)
		return "";
	std::string line = buffer.substr(0, pos);
	buffer.erase(0, pos + 2); // Remove the line including \r\n
	return line;
}

std::string Server::_concatenateTokens(const std::vector<std::string>& tokens, size_t startPos)
{
	std::string result;

	for (size_t i = startPos; i < tokens.size(); ++i) {
		result += tokens[i];
		if (i != tokens.size() - 1) {
			result += " ";
		}
	}
	result += ":";

	return result;
}

std::vector<std::string> Server::_splitString(const std::string & str, char separator) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string word;

    while (std::getline(ss, word, separator)) {
        result.push_back(word);
    }
    return result;
}

/// @brief parse the recipients from a PRIVMSG message
/// @param message 
/// @return vector of all the recipients
std::vector<std::string> Server::_parseReceivers(const std::string& message) {
    std::vector<std::string> receivers;

    std::string::size_type start = message.find(' ') + 1;
    std::string::size_type end = message.find(' ', start);
    
    if (start != std::string::npos && end != std::string::npos) {
        std::string receiverList = message.substr(start, end - start);
        receivers = _splitString(receiverList, ',');
    }

    return receivers;
}