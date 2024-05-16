/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 17:26:30 by mwallage          #+#    #+#             */
/*   Updated: 2024/05/16 18:26:33 by mwallage         ###   ########.fr       */
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
	pollfd		_socket;
	std::string _nickname;
	std::string _username;
	bool		_awaitingWelcomeMessage;
public:
	Client(int socketFd);
	Client(Client const &);
	Client & operator=(Client const &);
	~Client();

	void				setClientSocket(pollfd const &);
	pollfd const & 		getClientSocket() const;
	std::string const &	getNickname() const;
	void				setNickname(std::string const &);
	std::string const & getUsername() const;
	void				setUsername(std::string const &);
	bool 				isAwaitingWelcomeMessage() const;
	void				setAwaitingWelcomeMessage(bool isAwaiting);
};
