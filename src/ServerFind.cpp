#include "Server.hpp"

Client * Server::_findClient(std::string const & nickname)
{
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if ((*it)->getNickname() == nickname) {
			return *it;
		}
	}
	return NULL;
}

Client * Server::_findClient(int fd)
{
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if ((*it)->getClientSocket()->fd == fd) {
			return *it;
		}
	}
	return NULL;
}