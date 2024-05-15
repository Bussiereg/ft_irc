/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 17:32:08 by mwallage          #+#    #+#             */
/*   Updated: 2024/05/15 16:54:53 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

Client::Client(int socketFd)
{
	_socket.fd = socketFd;
	_socket.events = POLLIN | POLLOUT;
	_socket.revents = 0;
}

Client::Client(Client const & other)
	: _socket(other._socket), _nickname(other._nickname), _username(other._username)
{
}

Client & Client::operator=(Client const & other)
{
	if (this != &other)
	{
		_socket = other._socket;
		_nickname = other._nickname;
		_username = other._username;
	}
	return *this;
}

Client::~Client()
{
	std::cout << "Destructing client " << _socket.fd << std::endl;
	std::cout << "This might be only a copy" << std::endl;
}

void Client::setClientSocket(pollfd const & clientSocket)
{
	_socket = clientSocket;
}

pollfd const & 	Client::getClientSocket() const
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
