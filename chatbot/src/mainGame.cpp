#include "Gamebot.hpp"

int main(int argc, char **argv) {
	if( argc != 3){
		std::cerr << "./bot <port> <password>" << std::endl;
		return 0;
	}
	try {
		Gamebot gamebot(std::atoi(argv[1]), argv[2]);
	}
	catch(const std::exception& e) {
		std::cerr << "chatbot: " <<  e.what() << '\n';
	}
	return 0;
}
