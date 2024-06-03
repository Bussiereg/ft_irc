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
	int _numberToGuess;

	std::string		_serverName;
    unsigned int	_port;
    std::string		_nickBot;
    std::string		_userBot;
	std::string		_password;
	void 			_send_command(const std::string& cmd);
	int				_generateRandomNumber();
	void 			_wakeUp();
	void 			_handle_server_message(const std::string& message);
	std::string		_getMessageFromUser(const std::string& input);
public:
	bot(unsigned int, std::string);
	~bot();

	class botSocketCreationException : public std::exception {
		virtual const char *what() const throw();
	};

	class bothostnameException : public std::exception {
		virtual const char *what() const throw();
	};

	class botConnectException : public std::exception {
		virtual const char *what() const throw();
	};

};


#endif