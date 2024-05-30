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
			if ((*itch)->getChannelMode()['l'] == true && ((*itch)->getLimitUsers() <= (*itch)->getClientList().size())){
				client.appendResponse(ERR_CHANNELISFULL(client.getNickname(), (*itch)->getChannelName()));
				return ;
			}
			if ((*itch)->getChannelMode()['i'] == true){
				bool flag = false;
				std::vector<std::string>::iterator it;
				for (it = (*itch)->getInviteList().begin();  it != (*itch)->getInviteList().end(); ++it){
					if (*it == client.getNickname()){
						flag = true;
						break;
					}
				}
				if (!flag){
					client.appendResponse(ERR_INVITEONLYCHAN(client.getNickname(), (*itch)->getChannelName()));
					return ;
				}
			}
			if ((*itch)->getChannelPassword() == it->second){// Check password password
				(*itch)->setClientList(&client, false);
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

	std::vector<std::string> paramTopic = _splitString(input, ' ');
	if (paramTopic.size() < 2){
		std::string topic = "TOPIC";
		client.appendResponse(ERR_NEEDMOREPARAMS(topic));
		return ;
	}
	else if (paramTopic.size() >= 2){
		std::vector<Channel*>::iterator it;
		for (it = _channelList.begin(); it != _channelList.end(); ++it){
			if ((*it)->getChannelName() == paramTopic[1]){
				if ((*it)->getClientList().find(&client) == (*it)->getClientList().end() ){
					client.appendResponse(ERR_NOTONCHANNEL(_serverName ,(*it)->getChannelName()));
					return ;
				}
				else if (paramTopic.size() == 2){
					if ((*it)->getTopic().empty())
						client.appendResponse(RPL_NOTOPIC(client.getNickname(), client.getUsername(), client.gethostname(),  (*it)->getChannelName()));
					else
						client.appendResponse(RPL_TOPIC(client.getNickname(), client.getUsername(), client.gethostname(),  (*it)->getChannelName(), (*it)->getTopic()));
					return;
				}
				else if (paramTopic.size() >= 3){
					if (((*it)->getChannelMode()['t'] == false) || (((*it)->getChannelMode()['t'] == true) && ((*it)->getClientList()[&client] == true))){
						size_t colonpos = input.find(':');
						std::string newTopic = input.substr(colonpos + 1);
						(*it)->setTopic(newTopic);
						client.appendResponse(RPL_TOPIC(client.getNickname(), client.getUsername(), client.gethostname(),  (*it)->getChannelName(), (*it)->getTopic()));
						(*it)->relayMessage(client, RPL_TOPIC(client.getNickname(), client.getUsername(), client.gethostname(),  (*it)->getChannelName(), (*it)->getTopic()));
					}
					return ;
				}
			}
		}
	}
}

unsigned int Server::_findMode(char m){
	char setMode[] = "itkol";
	for (int i = 0; setMode[i] != '\0'; i++){
		if (m == setMode[i])
			return i;
	}
	return 5;
}

void	Server::modeInviteOnly(bool isOperator, Channel & channel){
		channel.setChannelMode('i', isOperator);
}

void	Server::modeTopic(bool isOperator, Channel & channel){
		channel.setChannelMode('t', isOperator);
}

void	Server::modeKeySet(bool isOperator, std::string key, Channel * channel){
	if (key.empty())
		channel->setChannelKey("");
	else
		channel->setChannelKey(key);
	channel->setChannelMode('k', isOperator);
}

void	Server::modeOperatorPriv(bool isOperator, std::string ope, Client & client, Channel * channel){
		if (channel->getClientList()[&client] == false){
			return ;
		}
		std::map<Client*, bool>::iterator it;
		for (it = channel->getClientList().begin(); it != channel->getClientList().end(); ++it){
			if (it->first->getNickname() == ope){
				channel->setClientList(it->first, isOperator);
				return ;
			}
		}
		if (it == channel->getClientList().end()){
			client.appendResponse(ERR_NOSUCHNICK(_serverName, client.getNickname(), ope));
		}
}

void	Server::modeSetUserLimit(bool isOperator, std::string limit, Channel & channel){
	if (!limit.empty())
		channel.setChannelLimit(std::atoi(limit.c_str()));
	channel.setChannelMode('l', isOperator);
}


void	Server::_handleModeCommand(Client & client, std::string & input){
	std::vector<std::string> modeSplit = _splitString(input, ' ');
	std::string mode = "MODE";
	Channel * channelInUse;

	if (modeSplit[0] != "MODE"){
		return ;
	}
	if (modeSplit.size() <= 2){
		std::vector<Channel*>::iterator itch;
		for (itch = _channelList.begin(); itch != _channelList.end(); ++itch){ // search if the channelname already exist
			if ((*itch)->getChannelName() == modeSplit[1]){
				channelInUse = *itch;
				break;
			}
		}
		if (itch == _channelList.end())
			client.appendResponse(ERR_NOSUCHCHANNEL(modeSplit[1]));
		else
			client.appendResponse(RPL_CHANNELMODEIS(_serverName, client.getNickname(), channelInUse->getChannelName(), channelInUse->getModeString()));
		return ;
	}
	else if (modeSplit.size() <= 4 ){
		std::vector<Channel*>::iterator itch;
		for (itch = _channelList.begin(); itch != _channelList.end(); ++itch){ // search if the channelname already exist
			if ((*itch)->getChannelName() == modeSplit[1]){
				channelInUse = *itch;
				break;
			}
		}
	
		if (itch == _channelList.end()){
			// client.appendResponse(RPL_CHANNELMODEIS(_serverName, client.getNickname(), "", "+i"));
			return ;
		}
		if ((*itch)->getClientList()[&client] == false)  // check if the client that try to change the mode is an operator
			return ;
		if (modeSplit.size() == 3 && modeSplit[2].size() == 1 && modeSplit[2][0] == 'b') // silence the return ban of the client
			return;

		bool isModeOn = false; 
		if (modeSplit[2][0] == '+')
			isModeOn = true;
		for (unsigned int i = 1; i < modeSplit[2].size(); i++){
			char m = modeSplit[2][i]; 
			if ((m == 'k' || m == 'o' || m == 'l') && (modeSplit.size() == 3) && isModeOn){
				client.appendResponse(ERR_NEEDMOREPARAMS(mode));
				return ;
			}
			else if ((m == 'o') && (modeSplit.size() == 3) && !isModeOn){
				client.appendResponse(ERR_NEEDMOREPARAMS(mode));
				return ;
			}
			switch (_findMode(m))
			{
			case INVITE_ONLY:
				modeInviteOnly(isModeOn, *channelInUse);
				break;
			case TOPIC:
				modeTopic(isModeOn, *channelInUse);
				break;
			case KEY:
				if (isModeOn){
					modeKeySet(isModeOn, modeSplit[3], channelInUse);
				}
				else{
					modeKeySet(isModeOn, "", channelInUse);
				}
				break;
			case OPERATOR_PRIVILEGE:
				modeOperatorPriv(isModeOn, modeSplit[3], client, channelInUse);
				break;
			case LIMIT:
				if (isModeOn)
					modeSetUserLimit(isModeOn, modeSplit[3], *channelInUse);
				else
					modeSetUserLimit(isModeOn, "", *channelInUse);
				break;		
			default:
				client.appendResponse(ERR_UNKNOWNMODE(_serverName, client.getNickname(), std::string(1, m)));
				return ;
				break;
			}
		}
		(*itch)->relayMessage(client, RPL_CHANNELMODEIS(_serverName, client.getNickname(), channelInUse->getChannelName(), channelInUse->getModeString()));
		client.appendResponse(RPL_CHANNELMODEIS(_serverName, client.getNickname(), channelInUse->getChannelName(), channelInUse->getModeString()));
	}
}


void	Server::_handleInviteCommand(Client & client, std::string & input){
	std::vector<std::string> inviteSplit = _splitString(input, ' ');
	Channel * channelInUse;
	if (inviteSplit.size() < 3){
		std::string invite = "INVITE";
		client.appendResponse(ERR_NEEDMOREPARAMS(invite));
		return ;
	}
	std::vector<Channel*>::iterator itch;
	for (itch = _channelList.begin(); itch != _channelList.end(); ++itch){ // search if the channelname already exist
		if ((*itch)->getChannelName() == inviteSplit[2]){
			channelInUse = *itch;
			break;
		}
	}
	if (itch == _channelList.end())
		return ;
	std::map<Client*, bool>::iterator clientInviter = channelInUse->getClientList().find(&client);
	if (clientInviter == channelInUse->getClientList().end()){
		return ;
	}
	std::vector<Client*>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); it++){
		if ((*it)->getNickname() == inviteSplit[1]){
			break;
		}
	}
	if (it == _clients.end()){
		client.appendResponse(ERR_NOSUCHNICK2(_serverName, inviteSplit[1]));
		return ;
	}
	if ((channelInUse->getChannelMode()['i'] == false) || ((channelInUse->getChannelMode()['i'] == true) && (channelInUse->getClientList()[clientInviter->first] == true)) ){
		channelInUse->setInviteList(inviteSplit[1]);
		(*it)->appendResponse(INVITE(client.getNickname(), client.getUsername(), client.getHostname(),  inviteSplit[1], channelInUse->getChannelName()));		
	}
}    

void 	Server::_handleKickCommand(Client & client, std::string & input){
	std::vector<std::string> kickSplit = _splitString(input, ' ');
	if (kickSplit.size() < 3){
		std::string kick = "KICK";
		return client.appendResponse(ERR_NEEDMOREPARAMS(kick));
	}
	else{
		std::string comment = "";
		if (kickSplit.size() >= 4){
			size_t colonpos = input.find(':');
			comment = input.substr(colonpos + 1);
		}
		std::vector<Channel*>::iterator it;
		Channel *channelInUse;
		bool channelExisting = false;
		for (it = _channelList.begin(); it != _channelList.end(); ++it){
			if ((*it)->getChannelName() == kickSplit[1]){
				channelExisting = true;
				channelInUse = *it;
				break;
			}
		}
		if (channelExisting == false){
			return client.appendResponse(ERR_NOSUCHCHANNEL(kickSplit[1]));
		}
		if (channelInUse->getClientList()[&client] == false){
			return client.appendResponse(ERR_CHANOPRIVSNEEDED(channelInUse->getChannelName()));
		}
		std::map<Client *, bool>::iterator it2;
		int clientToBeKickedOnChannel = false;
		Client * clientToBeKicked;
		for (it2 = channelInUse->getClientList().begin(); it2 != channelInUse->getClientList().end(); ++it2){
			if (it2->first->getNickname() == kickSplit[2]){
				clientToBeKickedOnChannel = true;
				clientToBeKicked = it2->first;
				break;
			}
		}
		if (clientToBeKickedOnChannel == false)
			return;
		channelInUse->removeClient(clientToBeKicked);
		clientToBeKicked->removeChannelJoined(channelInUse);
		channelInUse->relayMessage(client, KICK(client.getNickname(), client.getUsername(), client.gethostname(), channelInUse->getChannelName(), clientToBeKicked->getNickname() , comment));
		client.appendResponse(KICK(client.getNickname(), client.getUsername(), client.gethostname(), channelInUse->getChannelName(), clientToBeKicked->getNickname() , comment));
	}
	
}