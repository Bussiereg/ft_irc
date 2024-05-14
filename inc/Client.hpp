/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 17:26:30 by mwallage          #+#    #+#             */
/*   Updated: 2024/05/14 15:08:48 by mwallage         ###   ########.fr       */
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
	pollfd		_clientSocket;
	std::string _nickname;
	std::string _username;
public:
	Client(int socketFd);
	Client(Client const &);
	Client & operator=(Client const &);
	~Client();

	void				setClientPoll(pollfd const &);
	pollfd const & 		getClientPoll() const;
	std::string const &	getNickname() const;
	void				setNickname(std::string const &);
	std::string const & getUsername() const;
	void				setUsername(std::string const &);
};
