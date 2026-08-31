/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 12:14:48 by mamaimai          #+#    #+#             */
/*   Updated: 2026/08/31 12:14:52 by mamaimai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

Server::Server(int port)
    : _serverFd(-1), _port(port)
{
}

Server::~Server()
{
    if (_serverFd != -1)
        close(_serverFd);
}

void Server::start()
{
    /*
        1. Create the socket

        socket()
        ↓
        Create an IPv4 TCP socket
    */

    _serverFd = socket(AF_INET, SOCK_STREAM, 0);

    if (_serverFd == -1)
    {
        std::cerr << "socket() failed" << std::endl;
        return;
    }

    std::cout << "Socket created!" << std::endl;
    std::cout << "server_fd = " << _serverFd << std::endl;


    /*
        2. SO_REUSEADDR

        Allows us to reuse the local address/port when
        the previous connection may still be in TIME_WAIT.

        setsockopt() must be called before bind().
    */

    int opt = 1;

    if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR,
                   &opt, sizeof(opt)) == -1)
    {
        std::cerr << "setsockopt() failed" << std::endl;
        return;
    }

    std::cout << "SO_REUSEADDR enabled!" << std::endl;


    /*
        3. Make the socket non-blocking

        O_NONBLOCK means socket operations should not
        wait indefinitely.

        socket()
            ↓
        fcntl()
            ↓
        non-blocking socket
    */

    if (fcntl(_serverFd, F_SETFL, O_NONBLOCK) == -1)
    {
        std::cerr << "fcntl() failed" << std::endl;
        return;
    }

    std::cout << "Socket is non-blocking!" << std::endl;


    /*
        4. Prepare the server address

        sockaddr_in stores the IPv4 address information:

        sin_family → IPv4
        sin_port   → our port
        sin_addr   → local network interfaces
    */

    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_port = htons(_port);
    address.sin_addr.s_addr = htonl(INADDR_ANY);


    /*
        5. Bind the socket

        bind() associates our socket with
        the address and port.

        socket()
            ↓
        bind()
            ↓
        server_fd is associated with _port
    */

    if (bind(_serverFd,
             (struct sockaddr *)&address,
             sizeof(address)) == -1)
    {
        std::cerr << "bind() failed" << std::endl;
        return;
    }

    std::cout << "Bind successful!" << std::endl;


    /*
        6. Listen

        listen() puts the socket into listening mode.

        The second argument is the backlog:
        the queue of pending connection requests.
    */

    if (listen(_serverFd, 10) == -1)
    {
        std::cerr << "listen() failed" << std::endl;
        return;
    }

    std::cout << "Server is listening on port "
              << _port << std::endl;
}