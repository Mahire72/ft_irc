/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 22:02:53 by mamaimai          #+#    #+#             */
/*   Updated: 2026/10/13 14:28:49 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "It should be 3 parameters:  ./ircserv <port> <password> " << std::endl;
        return 1;
    }

    Server server(atoi(argv[1]), argv[2]);
    int port = server.getPort();
    if (port < 1024 || port > 65353)
    {
        std::cerr << "Error: invalid/not available port\n"
        << "Please enter a port in range 1024-65353\n";
        return 1;
    }
    std::cout <<"Hello from  my server! \n" << std::endl;
    server.start();

    return 0;
}
