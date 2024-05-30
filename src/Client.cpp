#include "Client.hpp"

Client::Client(pollfd * socket) : _socket(socket), _nickname(""), _isPassedWord(false), _isFullyAccepted(false), _passWordAttempted(false)
{
	_hostname = "localhost";
}

Client::Client(Client const & other)
	: _socket(other._socket), _nickname(other._nickname), _username(other._username), _hostname(other._hostname), _isPassedWord(other._isPassedWord), _isFullyAccepted(other._isFullyAccepted), _passWordAttempted(false)
{
}

Client & Client::operator=(Client const & other)
{
	if (this != &other)
	{
		_socket = other._socket;
		_nickname = other._nickname;
		_username = other._username;
		_hostname = other._hostname;
		_isPassedWord = other._isPassedWord;
		_isFullyAccepted = other._isFullyAccepted;
		_passWordAttempted = other._passWordAttempted;
	}
	return *this;
}

Client::~Client()
{
	delete _socket;
}

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

bool Client::isPassedWord() const
{
	return _isPassedWord;
}

bool Client::passWordAttempted() const
{
	return _passWordAttempted;
}

void Client::passWordAttempt()
{
	_passWordAttempted = true;
}


void Client::acceptPassword()
{
	_isPassedWord = true;
}

bool Client::isFullyAccepted() const
{
	return _isFullyAccepted;
}

void Client::acceptFully()
{
	_isFullyAccepted = true;
}

std::string const & Client::getResponse() const
{
	return _response;
}

void Client::appendResponse(std::string newMessage)
{
	_response.append(newMessage);
}

void Client::clearResponse()
{
	_response.clear();
}

std::string const & Client::gethostname() const{
	return _hostname;
}

std::vector<Channel*> & Client::getChannelJoined(){
	return _channelJoined;
}

void 	Client::removeChannelJoined(Channel * channel){
	std::vector<Channel*>::iterator it;
	for (it = _channelJoined.begin(); it != _channelJoined.end(); ++it){
		if ((*it) == channel)
			_channelJoined.erase(it);
	}
}

void 	Client::removeChannelJoined(Channel * channel){
	std::vector<Channel*>::iterator it;
	for (it = _channelJoined.begin(); it != _channelJoined.end(); ++it){
		if ((*it) == channel)
			_channelJoined.erase(it);
	}
}

std::set<int>  & Client::getContactList()
{
	return _contactList;
}

void				Client::setChannelJoined(Channel* channelToAdd){
	_channelJoined.push_back(channelToAdd);
}
