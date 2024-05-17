/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 16:49:06 by mwallage          #+#    #+#             */
/*   Updated: 2024/05/17 16:44:45 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

bool	Server::_isNickInUse(std::string const & nick)
{
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->getNickname() == nick) {
			return true;
		}
	}
	return false;
}

ssize_t	Server::_fillBuffer(size_t index, std::string & buffer)
{
	char temp[BUFFER_SIZE];

	ssize_t bytesRead = recv(_allSockets[index].fd, temp, BUFFER_SIZE, 0);
	if (bytesRead > 0)
		buffer.append(temp);
	return bytesRead;
}

std::string Server::_getNextLine(size_t & index, std::string & buffer)
{
	std::size_t pos;
    while ((pos = buffer.find("\r\n")) == std::string::npos) {
        if (_fillBuffer(index, buffer) <= 0) {
        	std::cerr << "[Server] Client fd " << _allSockets[index].fd << " just disconnected" << std::endl;
			index += _delClient(index);
			return "";
		}
	}
    std::string line = buffer.substr(0, pos);
    buffer.erase(0, pos + 2); // Remove the line including \r\n
    return line;
}