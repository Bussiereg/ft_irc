/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 17:32:08 by mwallage          #+#    #+#             */
/*   Updated: 2024/05/17 16:38:33 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

Client::Client(int socketFd) : _socketFd(socketFd), _isPassedWord(false),  _isFullyAccepted(false)
{
}

Client::Client(Client const & other)
	: _socketFd(other._socketFd), _nickname(other._nickname), _username(other._username), _hostname(other._hostname)
{
}

Client & Client::operator=(Client const & other)
{
	if (this != &other)
	{
		// socketFd cannot be copied because it is const
		_nickname = other._nickname;
		_username = other._username;
		_hostname = other._hostname;
	}
	return *this;
}

Client::~Client()
{
}

int const & 	Client::getClientFd() const
{
	return _socketFd;
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
