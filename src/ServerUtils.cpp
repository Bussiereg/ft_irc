#include "Server.hpp"

bool Server::_isNickInUse(std::string const & nick)
{
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if ((*it)->getNickname() == nick) {
			return true;
		}
	}
	return false;
}

bool Server::_isValidNickname(const std::string &nickname)
{
    if (nickname.empty() || nickname.size() > 9) {
        return false;
    }

    if (!std::isalpha(nickname[0])) {
        return false;
    }

    for (std::string::const_iterator it = nickname.begin() + 1; it != nickname.end(); ++it) {
        char c = *it;
        if (!(std::isalnum(c) || c == '-' || c == '[' || c == ']' || c == '\\' ||
              c == '`' || c == '^' || c == '_' || c == '{' || c == '}' || c == '|')) {
            return false;
        }
    }

    return true;
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