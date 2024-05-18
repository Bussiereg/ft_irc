/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 17:26:30 by mwallage          #+#    #+#             */
/*   Updated: 2024/05/18 17:29:38 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

class Client {
private:
	pollfd*			_socket;
	std::string 	_nickname;
	std::string 	_username;
	std::string 	_hostname;
	bool			_isPassedWord;
	bool			_isFullyAccepted;

	std::string		_response;
public:
	Client(pollfd * socket);
	Client(Client const &);
	Client & operator=(Client const &);
	~Client();

	pollfd const * 		getClientSocket() const;
	std::string const &	getNickname() const;
	void				setNickname(std::string const &);
	std::string const & getUsername() const;
	void				setUsername(std::string const &);
	bool				isPassedWord() const;
	void				acceptPassword();
	bool 				isFullyAccepted() const;
	void				acceptFully();

	std::string const & getResponse() const;
	void				appendResponse(std::string newMessage);
	void				clearResponse();
};
