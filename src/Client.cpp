/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 17:32:08 by mwallage          #+#    #+#             */
/*   Updated: 2024/05/12 17:55:38 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

Client::Client(int socket)
{
	_clientPoll.fd = socket;
}

Client::Client(Client const & other)
	: _clientPoll(other._clientPoll), _nickname(other._nickname), _username(other._username)
{
}

Client & Client::operator=(Client const & other)
{
	if (this != &other)
	{
		_clientPoll = other._clientPoll;
		_nickname = other._nickname;
		_username = other._username;
	}
	return *this;
}

Client::~Client() {}

void Client::setClientPoll(pollfd const & clientPoll)
{
	_clientPoll = clientPoll;
}

pollfd const & 	Client::getClientPoll() const
{
	return _clientPoll;
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
