/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 16:01:57 by mwallage          #+#    #+#             */
/*   Updated: 2024/05/13 15:57:32 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
#include "../inc/Client.hpp"
#include <csignal>

Server* g_server;

void signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		if (g_server == NULL)
			std::cout << "Server not active yet..." << std::endl;
	}
}

int main()
{
	try {
		signal(SIGINT, signal_handler);
		g_server = new(Server);
		std::cout << "Start polling..." << std::endl;
		g_server->startPolling();
		delete g_server;
	} catch (std::exception & e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
