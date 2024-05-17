/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 17:26:30 by mwallage          #+#    #+#             */
/*   Updated: 2024/05/17 13:34:40 by mwallage         ###   ########.fr       */
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
	int const		_socketFd;
	std::string 	_nickname;
	std::string 	_username;
	std::string 	_hostname;
	bool			_isPassedWord;
	bool			_isFullyAccepted;
public:
	Client(int socketFd);
	Client(Client const &);
	Client & operator=(Client const &);
	~Client();

	int const & 		getClientFd() const;
	std::string const &	getNickname() const;
	void				setNickname(std::string const &);
	std::string const & getUsername() const;
	void				setUsername(std::string const &);
	bool				isPassedWord() const;
	void				acceptPassword();
	bool 				isFullyAccepted() const;
	void				acceptFully();
};
