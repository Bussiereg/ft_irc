/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 17:26:30 by mwallage          #+#    #+#             */
/*   Updated: 2024/05/20 14:43:10 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include "Channel.hpp"

class Channel;

class Client {
private:
	pollfd *			_socket;
	std::string			_nickname;
	std::string			_username;
	std::string			_hostname;
	std::string			_realname;
	bool				_isPassedWord;
	bool				_isFullyAccepted;
	bool				_passWordAttempted;
	std::vector<int>	_contactList;

	std::vector<Channel>		_channelJoined;
	std::string					_response;
	std::string					_buffer;
//	bool			_isBufferFull;
public:
	Client(pollfd * socket);
	Client(Client const &);
	Client & operator=(Client const &);
	~Client();

	pollfd const * 		getClientSocket() const;
	std::string const &	getNickname() const;
	void				setNickname(std::string const &);
	std::string const & getUsername() const;
	std::string const & gethostname() const;
	std::vector<Channel> & getChannelJoined();

	void				setUsername(std::string const &);
	std::string const & getHostname() const;
	void				setHostname(std::string const &);
	std::string const & getRealname() const;
	void				setRealname(std::string const &);
	bool				passWordAttempted() const;
	void				passWordAttempt();
	bool				isPassedWord() const;
	void				acceptPassword();
	bool 				isFullyAccepted() const;
	void				acceptFully();

	std::string const &			getResponse() const;
	std::vector<int> const &	getPrvtmsgContactFDList();
	void						appendResponse(std::string newMessage);
	void						clearResponse();
};
