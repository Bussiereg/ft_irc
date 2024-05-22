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

	// check params
	std::vector<std::string> paramJoin = _splitString(message, ' ');
	std::vector<std::string> newChannelList = _splitString(paramJoin[1], ',');
	std::vector<std::string> newChannelPassword;
	// std::cout << newChannelList[0] << std::endl;
	if (paramJoin.size() == 1){
		std::string join = "JOIN";
		std::string response1 = ERR_NEEDMOREPARAMS(join);
		send(client.getClientSocket()->fd, response1.c_str(), response1.size(), 0);
		return ;
	}
	if (paramJoin.size() >= 3)
		newChannelPassword = _splitString(paramJoin[2], ',');
	
	for (unsigned long i = 0; i < newChannelList.size(); i++){
		if (newChannelList[i][0] != '&' && newChannelList[i][0] != '#'){
			std::string response2 = ERR_BADCHANMASK(client.getNickname(), newChannelList[i]);
			send(client.getClientSocket()->fd, response2.c_str(), response2.size(), 0);
			return ;
		}
	}

	//Create new channel or add client to channel
	std::vector<Channel>::iterator it;
	std::vector<Channel>::iterator ite = _channelList.end();
	bool flagNewChannel = true;
	for (it = _channelList.begin(); it != ite; ++it){
		if (it->getChannelName() == message){
			std::string response3 = RPL_TOPIC(client.getNickname(), client.getUsername(), client.gethostname(),  it->getChannelName(), it->getTopic());
			send(client.getClientSocket()->fd, response3.c_str(), response3.size(), 0);
			std::string reponse4 = RPL_NAMREPLY(client.getNickname(), client.getUsername(), client.gethostname(),  it->getChannelName());
			send(client.getClientSocket()->fd, reponse4.c_str(), reponse4.size(), 0);
			it->getClientList().insert(std::pair<Client*, bool>(&client, false));
			flagNewChannel = false;
		}
	}
	if (flagNewChannel){
		for (unsigned long i = 0; i < newChannelList.size(); i++){
			Channel *newChannel = new Channel(newChannelList[i], client);
			std::string response5 = RPL_NAMREPLY(client.getNickname(), client.getUsername(), client.gethostname(),  newChannel->getChannelName());
			send(client.getClientSocket()->fd, response5.c_str(), response5.size(), 0);
			_channelList.push_back(*newChannel);

		}
	}
	
}

std::vector<Channel> Server::getChannelList(){
	return _channelList;
}