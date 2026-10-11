/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 22:02:53 by mamaimai          #+#    #+#             */
/*   Updated: 2026/10/11 17:49:45 by mamaimai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

int main(int argc, char **argv)
{
    std::cout <<"Hello from  my server! \n" << std::endl;
    //std::cout << std::endl;
    
    if (argc != 3)
    {
        std::cout << "IL FAUT AVOIR 3 PARAMETRES. ./ircserv port secret " << std::endl;
        return 1;
    }
    
    Server server(atoi(argv[1]), argv[2]);

    server.start();

    return 0;
}
