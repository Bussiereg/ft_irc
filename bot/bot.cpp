#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

int sockfd;

void send_command(const std::string& cmd) {
    std::string full_cmd = cmd + "\r\n";
    send(sockfd, full_cmd.c_str(), full_cmd.length(), 0);
}

void handle_server_message(const std::string& message) {
    std::cout << "Server: " << message << std::endl;

    if (message.find("PING") != std::string::npos) {
        std::string response = "PONG " + message.substr(5);
        send_command(response);
    }

    if (message.find("PRIVMSG") != std::string::npos) {
        if (message.find("!hello") != std::string::npos) {
            send_command("PRIVMSG #channel :Hello, I am a bot!");
        }
    }
}

void irc_bot_main(const std::string& server, int port, const std::string& nick, const std::string& user, const std::string& pass) {
    struct sockaddr_in server_addr;
    struct hostent* host;

    if ((host = gethostbyname(server.c_str())) == nullptr) {
		std::cout << host << std::endl;
        std::cerr << "Error: Could not resolve hostname" << std::endl;
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error: Could not create socket" << std::endl;
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr = *((struct in_addr*) host->h_addr);
    memset(&(server_addr.sin_zero), '\0', 8);

    if (connect(sockfd, (struct sockaddr*) &server_addr, sizeof(struct sockaddr)) < 0) {
        std::cerr << "Error: Could not connect to server" << std::endl;
        exit(1);
    }

	send_command("PASS " + pass);
    send_command("NICK " + nick);
    send_command("USER " + user + " 0 * :" + user);
    char buffer[512];
    while (true) {
        memset(buffer, 0, 512);
        int bytes_received = recv(sockfd, buffer, 512, 0);
        if (bytes_received <= 0) {
            break;
        }
        std::string message(buffer, bytes_received);
        handle_server_message(message);
    }

    close(sockfd);
}

int main(int argc, char **argv) {
	if( argc != 3){
		return 0;
	}
    std::string server = "localhost";
    int port = std::atoi(argv[1]);
    std::string nick = "botnick";
    std::string user = "botuser";
	std::string password = argv[2];

    irc_bot_main(server, port, nick, user, password);

    return 0;
}
