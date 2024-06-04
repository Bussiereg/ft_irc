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
#include <vector>
#include <string>
#include <algorithm>

class bot
{
private:
	int 						_numberToGuess;
	std::string					_serverName;
    unsigned int				_port;
    std::string					_nickBot;
    std::string					_userBot;
	std::string					_password;
	void 						_sendCommand(const std::string& cmd);
	int							_generateRandomNumber();
	void 						_wakeUp();
	void 						_handleServerMessage(const std::string& message);
	std::string					_getMessageFromUser(const std::string& input);
	void						_greeting(std::string & sender, std::string & input);
	void 						_letsPlay(std::string & sender, std::string & input);
	std::vector<std::string> 	_listOfUser;
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