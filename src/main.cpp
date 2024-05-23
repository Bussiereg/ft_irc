/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 16:01:57 by mwallage          #+#    #+#             */
/*   Updated: 2024/05/23 19:32:34 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include <csignal>
#include <sstream>

bool g_quit = false;

void signal_handler(int )
{
	g_quit = true;
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
	
	signal(SIGINT, signal_handler);
	try {
		Server server(port, argv[2], "ft_irc");
		server.startPolling();
	} catch (std::exception & e) {
		std::cerr << "ircserver: " << e.what() << std::endl;
	}

	return 0;
}
