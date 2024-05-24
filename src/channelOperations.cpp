#include "Server.hpp"

void Server::_handleJoinCommand(Client & client, std::string & message){
	std::vector<std::string> joinSplit = _splitString(message, ' ');
	std::map<std::string, std::string> joinParams;

	if (joinSplit.size() <= 1){
		std::string join = "JOIN";
		std::string response1 = ERR_NEEDMOREPARAMS(join);
		client.appendResponse(response1);
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
		std::vector<Channel*>::iterator itch;
		for (itch = _channelList.begin(); itch != _channelList.end(); ++itch){ // search if the channelname already exist
			if ((*itch)->getChannelName() == it->first)
				break;
		}
		if (itch != _channelList.end()){ //channel already exist
			if ((*itch)->getChannelPassword() == it->second){// Check password password
				std::string response3 = RPL_TOPIC(client.getNickname(), client.getUsername(), client.gethostname(),  it->first, (*itch)->getTopic());
				client.appendResponse(response3);

				std::string usersInChannel = "";
				std::map<Client*, bool>::iterator it2;
				for (it2 = (*itch)->getClientList().begin(); it2 != (*itch)->getClientList().end(); ++it2) {
					if (it2->second == true){
						usersInChannel += '@' + it2->first->getNickname() + " ";
					}
					else{
						usersInChannel += it2->first->getNickname() + " ";
					}
				}
				
				std::string response4 = RPL_NAMREPLY(client.getNickname(), client.getUsername(), client.gethostname(),  (*itch)->getChannelName(), usersInChannel);
				client.appendResponse(response4);
				(*itch)->setClientList(client, false);
			}
			else{ // Password was wrong
				std::string response5 = ERR_BADCHANNELKEY(client.getNickname(), (*itch)->getChannelName());
				client.appendResponse(response5);
			}
		}
		else{ // new channel
			Channel *newChannel = new Channel(it->first, client);
			std::string usersInChannel = "@" + client.getNickname();
			std::string response6 = RPL_NAMREPLY(client.getNickname(), client.getUsername(), client.gethostname(),  newChannel->getChannelName(), usersInChannel);
			client.appendResponse(response6);
			newChannel->addClient(client, true);
			_channelList.push_back(newChannel); // add the channel to the SERVER channel list
			client.setChannelJoined(newChannel); // add the channel to the CLIENT channel joined list
		}
	}
	// std::cout << GREEN << "List of channels: " << std::endl;
    // int i = 0;
    // for (std::vector<Channel*>::iterator it = _channelList.begin(); it != _channelList.end(); it++) {
    //     std::cout << GREEN << "Channel " << i++ << ": " << (*it)->getChannelName() << std::endl;
    //     std::cout << BLUE << "    List of Clients: " << std::endl;
    //     std::map<Client*, bool>::iterator it2;
    //     for (it2 = (*it)->getClientList().begin(); it2 != (*it)->getClientList().end(); ++it2) {
    //         std::cout << "		Client Nickname: " << it2->first->getNickname() << std::endl;
    //     }
    // }
    // std::cout << RESET;
}

std::vector<Channel*> Server::getChannelList(){
	return _channelList;
}

void		Server::_handleTopicCommand(Client & client, std::string & input){
	std::vector<std::string> paramTopic = _splitString(input, ' ');
	if (paramTopic.size() < 2){
		std::string topic = "TOPIC";
		std::string response1 = ERR_NEEDMOREPARAMS(topic);
		client.appendResponse(response1);
		return ;
	}
	std::vector<Channel*>::iterator it;
	for (it = _channelList.begin(); it != _channelList.end(); ++it){
		if ((*it)->getChannelName() == paramTopic[1]){
			if (!(*it)->getClientList()[&client]){
				std::string response2 = ERR_NOTONCHANNEL(_serverName ,(*it)->getChannelName());
				client.appendResponse(response2);
				return ;
			}
			else{
				std::string response3;
				if ((*it)->getTopic().empty())
					response3 = RPL_NOTOPIC(client.getNickname(), client.getUsername(), client.gethostname(),  (*it)->getChannelName());
				else
					response3 = RPL_TOPIC(client.getNickname(), client.getUsername(), client.gethostname(),  (*it)->getChannelName(), (*it)->getTopic());	
				client.appendResponse(response3);
				return;
			}
		}

	}
}

void	Server::_handleModeCommand(Client & client, std::string & input){
	// std::vector<std::string> modeSplit = _splitString(input, ' ');
	
	// if (modeSplit.size() <= 2){
	// 	std::string mode = "MODE";
	// 	std::string response1 = ERR_NEEDMOREPARAMS(mode);
	// 	client.appendResponse(response1);
	// }
	// else if (modeSplit.size() == 3){
	// 	if (modeSplit[2][0] == '+'){
	// 		for (int i = 1; i < modeSplit[2].size(); i++){
	// 			_channelList[];
	// 		}
	// 	}
	// 	else if((modeSplit[1][0] == '-')){

	// 	}
	// }
	// else if (modeSplit.size() == 4){
	// }
	(void)client;	
	(void)input;

}