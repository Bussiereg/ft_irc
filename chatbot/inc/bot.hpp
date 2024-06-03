#ifndef BOT_HPP
#define BOT_HPP

#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <unistd.h>
#include <ctime>

class bot
{
private:
	unsigned int _numberToGuess;

	std::string		_serverName;
    unsigned int	_port;
    std::string		_nickBot;
    std::string		_userBot;
	std::string		_password;
	std::string 	_extractUsername(const std::string& message);
	void 			_send_command(const std::string& cmd);
	unsigned int	_generateRandomNumber();
	void 			_wakeUp();
	void 			_handle_server_message(const std::string& message);
public:
	bot(unsigned int, std::string);
	~bot();

	class SocketCreationException : public std::exception {
		virtual const char *what() const throw();
	};

	class hostnameException : public std::exception {
		virtual const char *what() const throw();
	};

	class ConnectException : public std::exception {
		virtual const char *what() const throw();
	};


	class SocketBindingException : public std::exception {
		virtual const char* what() const throw();
	};

	class SocketListeningException : public std::exception {
		virtual const char* what() const throw();
	};
};

bot::~bot()
{
}


#endif