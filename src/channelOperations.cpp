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
	std::vector<std::string> joinSplit = _splitString(message, ' ');
	std::map<std::string, std::string> joinParams;

	if (joinSplit.size() <= 1){
		std::string join = "JOIN";
		std::string response1 = ERR_NEEDMOREPARAMS(join);
		send(client.getClientSocket()->fd, response1.c_str(), response1.size(), 0);
		return ;
	}
	else if (joinSplit.size() == 2){
		std::vector<std::string> newChannelList = _splitString(joinSplit[1], ',');
		for (unsigned long i = 0; i < newChannelList.size(); i++){
			joinParams.insert(std::pair<std::string, std::string>(newChannelList[i], ""));
		}
	}
	else if (joinSplit.size() >= 3){
		std::vector<std::string> newChannelList = _splitString(joinSplit[1], ',');
		std::vector<std::string> newChannelKey = _splitString(joinSplit[2], ',');
		for (unsigned long i = 0; i < newChannelList.size(); i++){
			if (newChannelKey.size() <= i) // if there is more channel than password, don't add he password
				joinParams.insert(std::pair<std::string, std::string>(newChannelList[i], ""));
			else
				joinParams.insert(std::pair<std::string, std::string>(newChannelList[i], newChannelKey[i]));
		}
	}

	for (std::map<std::string, std::string>::iterator it = joinParams.begin(); it != joinParams.end(); ++it){
		std::vector<Channel>::iterator itch;
		for (itch = _channelList.begin(); itch != _channelList.end(); ++itch){ // search if the channelname already exist
			if (itch->getChannelName() == it->first)
				break;
		}
		if (itch != _channelList.end()){ //channel already exist
			if (itch->getChannelPassword() == it->second){// Check password password
				std::string response3 = RPL_TOPIC(client.getNickname(), client.getUsername(), client.gethostname(),  itch->getChannelName(), itch->getTopic());
				send(client.getClientSocket()->fd, response3.c_str(), response3.size(), 0);
				std::string reponse4 = RPL_NAMREPLY(client.getNickname(), client.getUsername(), client.gethostname(),  itch->getChannelName());
				send(client.getClientSocket()->fd, reponse4.c_str(), reponse4.size(), 0);
				itch->getClientList().insert(std::pair<Client*, bool>(&client, false));
			}
			else{ // Password was wrong
				std::string reponse5 = ERR_BADCHANNELKEY(client.getNickname(), itch->getChannelName());
				send(client.getClientSocket()->fd, reponse5.c_str(), reponse5.size(), 0);
			}
		}
		else{ // new channel
			Channel newChannel(it->first, client);
			std::string response6 = RPL_NAMREPLY(client.getNickname(), client.getUsername(), client.gethostname(),  newChannel.getChannelName());
			send(client.getClientSocket()->fd, response6.c_str(), response6.size(), 0);
			newChannel.getClientList().insert(std::pair<Client*, bool>(&client, true)); // add the client as operator in the CHANNEL client list
			_channelList.push_back(newChannel); // add the channel to the SERVER channel list
			client.getChannelJoined().push_back(newChannel); // add the channel to the CLIENT channel joined list
		}
	}
}

std::vector<Channel> Server::getChannelList(){
	return _channelList;
}

void		Server::_handleTopicCommand(Client & client, std::string & input){
	std::vector<std::string> paramTopic = _splitString(input, ' ');
	if (paramTopic.size() < 2){
		std::string topic = "TOPIC";
		std::string response1 = ERR_NEEDMOREPARAMS(topic);
		send(client.getClientSocket()->fd, response1.c_str(), response1.size(), 0);
		return ;
	}
	std::vector<Channel>::iterator it;
	for (it = _channelList.begin(); it != _channelList.end(); ++it){
		if (it->getChannelName() == paramTopic[1]){
			if (!it->getClientList()[&client]){
				std::string response2 = ERR_NOTONCHANNEL(it->getChannelName());	
				send(client.getClientSocket()->fd, response2.c_str(), response2.size(), 0);
				return ;
			}
			else{
				std::string response3;
				if (it->getTopic().empty())
					response3 = RPL_NOTOPIC(client.getNickname(), client.getUsername(), client.gethostname(),  it->getChannelName());
				else
					response3 = RPL_TOPIC(client.getNickname(), client.getUsername(), client.gethostname(),  it->getChannelName(), it->getTopic());	
				send(client.getClientSocket()->fd, response3.c_str(), response3.size(), 0);
				return;
			}
		}

	}
}