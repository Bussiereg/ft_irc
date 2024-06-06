#include "Client.hpp"

Client::Client(pollfd *socket) : _socket(socket), _nickname(""), _hostname(HOSTNAME), _isPassedWord(false), _isFullyAccepted(false)
{
}

Client::Client(Client const &other)
	: _socket(other._socket), _nickname(other._nickname), _username(other._username), _hostname(other._hostname), _isPassedWord(other._isPassedWord), _isFullyAccepted(other._isFullyAccepted)
{
}

Client &Client::operator=(Client const &other)
{
	if (this != &other)
	{
		_socket = other._socket;
		_nickname = other._nickname;
		_username = other._username;
		_hostname = other._hostname;
		_isPassedWord = other._isPassedWord;
		_isFullyAccepted = other._isFullyAccepted;
	}
	return *this;
}

Client::~Client()
{
	delete _socket;
}

bool Client::isPassedWord() const
{
	return _isPassedWord;
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
