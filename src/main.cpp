/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 16:01:57 by mwallage          #+#    #+#             */
/*   Updated: 2024/05/14 15:34:12 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
#include "../inc/Client.hpp"
#include <csignal>
#include <sstream>

Server* g_server;

void signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		if (g_server) {
			delete g_server;
			exit(0);
		}
		else
			std::cout << "Server not active yet..." << std::endl;
	}
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		std::cout << "Usage: ./ircserv <port> <password>";
		return 1;
	}

	std::istringstream ss(argv[1]);
	int port;
	if (!(ss >> port))
  		std::cerr << "Invalid port number: " << argv[1] << std::endl;
	else if (!ss.eof())
  		std::cerr << "Trailing characters after port number: " << argv[1] << std::endl;
	
	try {
		signal(SIGINT, signal_handler);
		g_server = new Server(port, argv[2]);
		std::cout << "Start polling..." << std::endl;
		g_server->startPolling();
	//	delete g_server;
	} catch (std::exception & e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
