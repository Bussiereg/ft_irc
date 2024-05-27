#include "Server.hpp"

void Channel::getUserListInChannel(std::string & usersInChannel){
	std::map<Client*, bool>::iterator it2;
	for (it2 = getClientList().begin(); it2 != getClientList().end(); ++it2) {
		if (it2->second == true)
			usersInChannel += '@' + it2->first->getNickname() + " ";
		else
			usersInChannel += it2->first->getNickname() + " ";
	}
}

void Server::_createJoinmap(Client & client, std::string & message, std::map<std::string, std::string> & joinParams){
	std::vector<std::string> joinSplit = _splitString(message, ' ');

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
			if (newChannelKey.size() <= i) // if there is more channel than password, don't add the passwords
				joinParams.insert(std::pair<std::string, std::string>(newChannelList[i], ""));
			else
				joinParams.insert(std::pair<std::string, std::string>(newChannelList[i], newChannelKey[i]));
		}
	}
}

std::vector<Channel*>::iterator Server::isChannelAlreadyExisting(std::string rhs){
	std::vector<Channel*>::iterator itch;
	for (itch = _channelList.begin(); itch != _channelList.end(); ++itch){ // search if the channelname already exist
		if ((*itch)->getChannelName() == rhs)
			break;
	}
	return itch;
}

void Server::_handleJoinCommand(Client & client, std::string & message){
	std::map<std::string, std::string> joinParams;
	std::string usersInChannel;

	_createJoinmap(client, message, joinParams);
	for (std::map<std::string, std::string>::iterator it = joinParams.begin(); it != joinParams.end(); ++it){
		std::vector<Channel*>::iterator itch = isChannelAlreadyExisting(it->first);
		if (itch != _channelList.end()){ //channel already exist
			if ((*itch)->getChannelPassword() == it->second){// Check password password
				(*itch)->setClientList(client, false);
				(*itch)->getUserListInChannel(usersInChannel);
				client.appendResponse(RPL_TOPIC(client.getNickname(), client.getNickname(), client.gethostname(),  it->first, (*itch)->getTopic()));
				client.appendResponse(RPL_NAMREPLY( _serverName, client.getNickname(), (*itch)->getChannelName(), usersInChannel));
				client.appendResponse(RPL_ENDOFNAMES(_serverName, client.getNickname(), (*itch)->getChannelName()));
			}
			else // Password was wrong
				client.appendResponse(ERR_BADCHANNELKEY(client.getNickname(), (*itch)->getChannelName()));
		}
		else{ // new channel
			Channel *newChannel = new Channel(it->first, client);
			_channelList.push_back(newChannel); // add the channel to the SERVER channel list
			client.setChannelJoined(newChannel); // add the channel to the CLIENT channel joined list
			newChannel->addClient(client, true); // add the client to the CHANNEL client list
			newChannel->getUserListInChannel(usersInChannel);
			client.appendResponse(RPL_NAMREPLY (_serverName, client.getNickname(), newChannel->getChannelName(), usersInChannel));
			client.appendResponse(RPL_ENDOFNAMES(_serverName, client.getNickname(), newChannel->getChannelName()));	
		}
	}
}

std::vector<Channel*> Server::getChannelList(){
	return _channelList;
}

void		Server::_handleTopicCommand(Client & client, std::string & input){
	(void)input;
	(void)client;
	// std::vector<std::string> paramTopic = _splitString(input, ' ');
	// if (paramTopic.size() < 2){
	// 	std::string topic = "TOPIC";
	// 	std::string response1 = ERR_NEEDMOREPARAMS(topic);
	// 	client.appendResponse(response1);
	// 	return ;
	// }
	// std::vector<Channel*>::iterator it;
	// for (it = _channelList.begin(); it != _channelList.end(); ++it){
	// 	if ((*it)->getChannelName() == paramTopic[1]){
	// 		if (!(*it)->getClientList()[&client]){
	// 			std::string response2 = ERR_NOTONCHANNEL(_serverName ,(*it)->getChannelName());
	// 			client.appendResponse(response2);
	// 			return ;
	// 		}
	// 		else{
	// 			std::string response3;
	// 			if ((*it)->getTopic().empty())
	// 				response3 = RPL_NOTOPIC(client.getNickname(), client.getUsername(), client.gethostname(),  (*it)->getChannelName());
	// 			else
	// 				response3 = RPL_TOPIC(client.getNickname(), client.getUsername(), client.gethostname(),  (*it)->getChannelName(), (*it)->getTopic());	
	// 			client.appendResponse(response3);
	// 			return;
	// 		}
	// 	}

	// }
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