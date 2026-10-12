/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 22:02:53 by mamaimai          #+#    #+#             */
/*   Updated: 2026/10/12 15:20:49 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

int main(int argc, char **argv)
{
    std::cout <<"Hello from  my server! \n" << std::endl;

    if (argc != 3)
    {
        std::cout << "It should be 3 parameters:  ./ircserv <port> <password> " << std::endl;
        return 1;
    }

    Server server(atoi(argv[1]), argv[2]);

    server.start();

    return 0;
}
