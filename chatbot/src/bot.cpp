#include "bot.hpp"

int sockfd;

void bot::_send_command(const std::string& cmd) {
    std::string full_cmd = cmd + "\r\n";
    send(sockfd, full_cmd.c_str(), full_cmd.length(), 0);
}

std::string bot::_getMessageFromUser(const std::string& input) {
    std::string result;
    std::string::size_type firstColonPos = input.find(':');
    if (firstColonPos == std::string::npos)
        return ""; // No colon found
    std::string::size_type secondColonPos = input.find(':', firstColonPos + 1);
    if (secondColonPos == std::string::npos)
        return ""; // Only one colon found
    result = input.substr(secondColonPos + 1);
    result = result.substr(0, result.find('\r'));
    return result;
}

void bot::_handle_server_message(const std::string& message) {
    std::cout << "Server: " << message << std::endl;
    static bool greeting = false;

    if (message.find("PING") != std::string::npos) {
        std::string response = "PONG " + message.substr(5);
        _send_command(response);
    }
    if (message.find("PRIVMSG") != std::string::npos) {
		std::string sender = message.substr(1, message.find("!") - 1);
        std::string input = _getMessageFromUser(message);
        if (greeting == false){
            if (input == "hello") {
                greeting = true;
                _send_command("PRIVMSG " + sender + " :Hello, I am a bot!");
                _send_command("PRIVMSG " + sender + " :Let's play a game. I'll think of a number from 0 to 20 and you have to guess it.");
            }
            else if (input == "yo") {
                greeting = true;
                _send_command("PRIVMSG " + sender + " :Yooooo man, I am a bot!");
                _send_command("PRIVMSG " + sender + " :Let's play a game. I'll think of a number from 0 to 20 and you have to guess it.");
            }
            else{
                greeting = true;
                _send_command("PRIVMSG " + sender + " :Hey! I am a bot!");
                _send_command("PRIVMSG " + sender + " :Let's play a game. I'll think of a number from 0 to 20 and you have to guess it.");
            }
        }
        else{
            if (std::atoi(input.c_str()) < _numberToGuess){
                _send_command("PRIVMSG " + sender + " :more!");
            }
            else if (std::atoi(input.c_str()) > _numberToGuess){
                _send_command("PRIVMSG " + sender + " :less!");
            }
            else if (std::atoi(input.c_str()) == _numberToGuess){
                _send_command("PRIVMSG " + sender + " :Congratulation!");
                _send_command("PRIVMSG " + sender + " :Let's play again!");
                _send_command("PRIVMSG " + sender + " :this so much fun! :D");
                _numberToGuess = _generateRandomNumber();
            }
        }
    }
}

void bot::_wakeUp() {
    struct sockaddr_in server_addr;
    struct hostent* host;

    if ((host = gethostbyname(_serverName.c_str())) == NULL) {
		std::cout << host << std::endl;
        throw bothostnameException();
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        throw botSocketCreationException();
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(_port);
    server_addr.sin_addr = *((struct in_addr*) host->h_addr);
    memset(&(server_addr.sin_zero), '\0', 8);

    if (connect(sockfd, (struct sockaddr*) &server_addr, sizeof(struct sockaddr)) < 0) {
        throw botConnectException();
    }

	_send_command("PASS " + _password);
    _send_command("NICK " + _nickBot);
    _send_command("USER " + _userBot + " 0 * :" + _userBot);
    char buffer[512];
    while (true) {
        memset(buffer, 0, 512);
        int bytes_received = recv(sockfd, buffer, 512, 0);
        if (bytes_received <= 0) {
            break;
        }
        std::string message(buffer, bytes_received);
		std::cout << "message: " << message << std::endl;
        _handle_server_message(message);
    }

    close(sockfd);
}

int bot::_generateRandomNumber() {
    std::srand(std::time(0));
    return std::rand() % 21;
}

bot::bot(unsigned int port, std::string pass)
        :
            _serverName("localhost"),
            _port(port),
            _nickBot("Botbot"),
            _userBot("botuser"),
            _password(pass)
{
    _numberToGuess = _generateRandomNumber();
    _wakeUp();
}

const char *bot::botSocketCreationException::what() const throw()
{
	return "Chatbot: error creating socket";
}

const char *bot::botConnectException::what() const throw()
{
	return "Chatbot: Could not connect to server";
}

const char *bot::bothostnameException::what() const throw()
{
	return "Chatbot: hostname has not been found";
}

bot::~bot()
{
}