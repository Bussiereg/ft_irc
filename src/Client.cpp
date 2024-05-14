/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 17:32:08 by mwallage          #+#    #+#             */
/*   Updated: 2024/05/14 15:09:14 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

Client::Client(int socket)
{
	_clientSocket.fd = socket;
}

Client::Client(Client const & other)
	: _clientSocket(other._clientSocket), _nickname(other._nickname), _username(other._username)
{
}

Client & Client::operator=(Client const & other)
{
	if (this != &other)
	{
		_clientSocket = other._clientSocket;
		_nickname = other._nickname;
		_username = other._username;
	}
	return *this;
}

Client::~Client()
{
	close(_clientSocket.fd);
	std::cout << "Destructing client..." << std::endl;
}

void Client::setClientPoll(pollfd const & clientPoll)
{
	_clientSocket = clientPoll;
}

pollfd const & 	Client::getClientPoll() const
{
	return _clientSocket;
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
