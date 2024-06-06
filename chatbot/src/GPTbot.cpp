#include "GPTbot.hpp"

void GPTbot::_sendCommand(const std::string& cmd)
{
	std::string full_cmd = cmd + "\r\n";
	send(_sockfd, full_cmd.c_str(), full_cmd.length(), 0);
}

std::string GPTbot::_getMessageFromUser(const std::string& input)
{
	std::string result;
	std::string::size_type firstColonPos = input.find(':');
	if (firstColonPos == std::string::npos)
		return "";
	std::string::size_type secondColonPos = input.find(':', firstColonPos + 1);
	if (secondColonPos == std::string::npos)
		return "";
	result = input.substr(secondColonPos + 1);
	result = result.substr(0, result.find('\r'));
	return result;
}

std::string GPTbot::_readResponseFromFile(const char* filename)
{
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Error opening file: " << filename << std::endl;
		return "";
	}
	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	return content.substr(2);
}

std::string GPTbot::_send_to_chatgpt(std::string& input)
{
	 pid_t pid = fork();

	if (pid < 0) {
		std::cerr << "Error forking process." << std::endl;
		throw std::runtime_error("Error forking process.");
	}

	if (pid == 0) {
		const char* argv[] = {"./botGPT.sh", input.c_str(), NULL};
		if (execv("./botGPT.sh", const_cast<char**>(argv)) == -1) {
			std::cerr << "Error executing the script." << std::endl;
			_exit(EXIT_FAILURE);
		}
	} else {
		int status;
		waitpid(pid, &status, 0);

		if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
			return _readResponseFromFile("response.txt");
		} else {
			std::cerr << "Script execution failed." << std::endl;
			throw std::runtime_error("Script execution failed.");
		}
	}
	return _readResponseFromFile("response.txt");
}

void GPTbot::_handleServerMessage(const std::string& message) 
{
	if (message.find("PRIVMSG") != std::string::npos) {
		std::string sender = message.substr(1, message.find("!") - 1);
		std::string input = _getMessageFromUser(message);
		std::string response = _send_to_chatgpt(input);
		_sendCommand("PRIVMSG " + sender + " :" + response);
	}
}


void GPTbot::_wakeUp() {
	struct sockaddr_in server_addr;
	struct hostent* host;

	if ((host = gethostbyname(_serverName.c_str())) == NULL) {
		throw bothostnameException();
	}
	_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockfd < 0) {
		throw botSocketCreationException();
	}
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(_port);
	server_addr.sin_addr = *((struct in_addr*) host->h_addr);
	memset(&(server_addr.sin_zero), '\0', 8);

	if (connect(_sockfd, (struct sockaddr*) &server_addr, sizeof(struct sockaddr)) < 0) {
		throw botConnectException();
	}
	_sendCommand("PASS " + _password);
	_sendCommand("NICK " + _nickBot);
	_sendCommand("USER " + _userBot + " 0 * :" + _userBot);
	char buffer[512];
	while (true) {
		memset(buffer, 0, 512);
		int bytes_received = recv(_sockfd, buffer, 512, 0);
		if (bytes_received <= 0) {
			std::cout << "Connection closed" << std::endl;
			break;
		}
		std::string message(buffer, bytes_received);
		_handleServerMessage(message);
	}
	close(_sockfd);
}

GPTbot::GPTbot(unsigned int port, std::string pass)
		:
			_serverName("localhost"),
			_port(port),
			_nickBot("gptbot"),
			_userBot("botuser"),
			_password(pass)
{
	_wakeUp();
}

const char *GPTbot::botSocketCreationException::what() const throw()
{
	return "Chatbot: error creating socket";
}

const char *GPTbot::botConnectException::what() const throw()
{
	return "Chatbot: Could not connect to server";
}

const char *GPTbot::bothostnameException::what() const throw()
{
	return "Chatbot: hostname has not been found";
}

GPTbot::~GPTbot()
{
}