#include "Client.hpp"


pollfd const * Client::getClientSocket() const
{
	return _socket;
}

std::string const &	Client::getNickname() const
{
	return _nickname;
}

void Client::setNickname(std::string const & nick)
{
	_nickname = nick;
}

std::string const & Client::getUsername() const
{
	return _username;
}

void Client::setUsername(std::string const & username)
{
	_username = username;
}

std::string const & Client::getHostname() const
{
	return _hostname;
}

void Client::setHostname(std::string const & hostname)
{
	_hostname = hostname;
}

std::string const & Client::getRealname() const
{
	return _realname;
}

void Client::setRealname(std::string const & realname)
{
	_realname = realname;
}

std::string const & Client::getResponse() const
{
	return _response;
}

void Client::appendResponse(std::string newMessage)
{
	_response.append(newMessage + "\r\n");
}

std::string const &	Client::getBuffer() const
{
	return _buffer;
}

void Client::appendBuffer(std::string const & str)
{
	_buffer += str;
}

void Client::clearBuffer()
{
	_buffer.clear();
}

std::string Client::PopFirstLineBuffer()
{
	size_t pos = _buffer.find("\r\n");
	if (pos == std::string::npos)
		return "";
	std::string newLine = _buffer.substr(0, pos);
	_buffer.erase(0, pos + 2);
	return newLine;
}

void Client::clearResponse()
{
	_response.clear();
}

std::vector<Channel*> & Client::getChannelJoined(){
	return _channelJoined;
}

void Client::removeChannelJoined(Channel * channel){
	std::vector<Channel*>::iterator it;
	for (it = _channelJoined.begin(); it != _channelJoined.end(); ++it){
		if ((*it) == channel){
			_channelJoined.erase(it);
			break;
		}
	}
}

void Client::setChannelJoined(Channel* channelToAdd){
	_channelJoined.push_back(channelToAdd);
}
