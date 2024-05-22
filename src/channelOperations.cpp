#include "Server.hpp"

std::vector<std::string> Server::_splitString(const std::string & str, char separator) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string word;

    while (std::getline(ss, word, separator)) {
        result.push_back(word);
    }
    return result;
}

void Server::_handleJoinCommand(Client & client, std::string & message){
	std::vector<std::string> paramJoin = _splitString(message, ' ');
	std::vector<std::string> channelList = _splitString(message, ',');
	for (unsigned long i = 0; i < channelList.size(); i++){
		if (channelList[i][0] != '&' || channelList[i][0] != '#'){
			std::string response = ERR_BADCHANMASK(client.getNickname(), channelList[i]);
			send(client.getClientSocket()->fd, response.c_str(), response.size(), 0);
		}
	}
}