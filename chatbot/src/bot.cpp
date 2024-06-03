#include "bot.hpp"

int sockfd;

void bot::_send_command(const std::string& cmd) {
    std::string full_cmd = cmd + "\r\n";
    std::cout << "example" << std::endl;
    send(sockfd, full_cmd.c_str(), full_cmd.length(), 0);
}

std::string bot::_extractUsername(const std::string& message) {
    size_t startPos = message.find(":");
    if (startPos == std::string::npos) {
        return ""; // No ':' found
    }

    size_t endPos = message.find("!", startPos);
    if (endPos == std::string::npos) {
        return ""; // No '!' found
    }

    // Extract the substring between ':' and '!'
    return message.substr(startPos + 1, endPos - startPos - 1);
}

void bot::_handle_server_message(const std::string& message) {
    std::cout << "Server: " << message << std::endl;

    if (message.find("PING") != std::string::npos) {
        std::string response = "PONG " + message.substr(5);
        _send_command(response);
    }
    if (message.find("PRIVMSG") != std::string::npos) {
		std::string sender = _extractUsername(message);
        if (message.find("!hello") != std::string::npos) {
            _send_command("PRIVMSG " + sender + " #channel :Hello, I am a bot!");
        }
    }
}

void bot::_wakeUp() {
    struct sockaddr_in server_addr;
    struct hostent* host;

    if ((host = gethostbyname(_serverName.c_str())) == NULL) {
		std::cout << host << std::endl;
        throw hostnameException();
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        throw SocketCreationException();
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(_port);
    server_addr.sin_addr = *((struct in_addr*) host->h_addr);
    memset(&(server_addr.sin_zero), '\0', 8);

    if (connect(sockfd, (struct sockaddr*) &server_addr, sizeof(struct sockaddr)) < 0) {
        throw ConnectException();
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

unsigned int bot::_generateRandomNumber() {
    std::srand(std::time(0));
    return std::rand() % 21;
}

bot::bot(unsigned int port, std::string pass)
        : _nickBot("Botbot"),
            _userBot("botuser"),
            _serverName("localhost"),
            _password(pass),
            _port(port)
{
    _numberToGuess = _generateRandomNumber();
    _wakeUp();
}

const char *bot::SocketCreationException::what() const throw()
{
	return "Chatbot: error creating socket";
}

const char *bot::ConnectException::what() const throw()
{
	return "Chatbot: Could not connect to server";
}

const char *bot::hostnameException::what() const throw()
{
	return "Chatbot: hostname has not been found";
}

const char *bot::SocketBindingException::what() const throw()
{
	return "Chatbot: error binding socket";
}

const char *bot::SocketListeningException::what() const throw()
{
	return "Chatbot: error listening on socket";
}


int main(int argc, char **argv) {
	if( argc != 3){
        std::cerr << "./bot <port> <password>" << std::endl;
		return 0;
	}
    bot chatbot(std::atoi(argv[1]), argv[2]);
    return 0;
}
