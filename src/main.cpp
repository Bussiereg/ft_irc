/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 16:01:57 by mwallage          #+#    #+#             */
/*   Updated: 2024/05/13 14:32:15 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
#include "../inc/Client.hpp"
#include <csignal>

int main()
{
	try {
		Server server;
		std::cout << "Start polling..." << std::endl;
		server.startPolling();
	} catch (std::exception & e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
