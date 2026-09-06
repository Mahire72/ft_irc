/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerNetwork.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 15:15:58 by mamaimai          #+#    #+#             */
/*   Updated: 2026/09/06 15:20:46 by mamaimai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

// This file's responsibility is: Get the server socket ready to accept connections

void Server::setupSocket()
{
    _serverFd = socket(AF_INET, SOCK_STREAM, 0);

    if (_serverFd == -1)
    {
        std::cerr << "socket() failed" << std::endl;
        return;
    }

    std::cout << "Socket created!" << std::endl;
    std::cout << "server_fd = " << _serverFd << std::endl;

    int opt = 1;

    if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR,
                   &opt, sizeof(opt)) == -1)
    {
        std::cerr << "setsockopt() failed" << std::endl;
        return;
    }

    std::cout << "SO_REUSEADDR enabled!" << std::endl;

    if (fcntl(_serverFd, F_SETFL, O_NONBLOCK) == -1)
    {
        std::cerr << "fcntl() failed" << std::endl;
        return;
    }

    std::cout << "Socket is non-blocking!" << std::endl;
}

void Server::setupAddress()
{
    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_port = htons(_port);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(_serverFd,
             (struct sockaddr *)&address,
             sizeof(address)) == -1)
    {
        std::cerr << "bind() failed" << std::endl;
        return;
    }

    std::cout << "Bind successful!" << std::endl;
}

void Server::startListening()
{
    if (listen(_serverFd, 10) == -1)
    {
        std::cerr << "listen() failed" << std::endl;
        return;
    }

    std::cout << "Server is listening on port "
              << _port << std::endl;
}