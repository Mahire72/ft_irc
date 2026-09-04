/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 12:14:48 by mamaimai          #+#    #+#             */
/*   Updated: 2026/09/04 18:40:32 by mamaimai         ###   ########.fr       */
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

    // pollfd()
    struct pollfd pfd;
    pfd.fd = _serverFd;
    pfd.events = POLLIN;
    pfd.revents = 0;

    _pollFds.push_back(pfd);

    /*
    programme up to here is fine, server_fd = 3 does not means 3 connections.
    it simply means the operating system assigned file descriptor number 3 to your listenning socket.
    */
    while (true)
    {
        int ret = poll(&_pollFds[0], _pollFds.size(), -1);

        if (ret == -1)
        {
            std::cerr << "poll() failed" << std::endl;
            return;
        }

        if (_pollFds[0].revents & POLLIN)
        {
            std::cout << "New connection is waiting!" << std::endl;
        }
    }
    /* poll() wakes up because a connection is waiting in the listenning socket's queue. but we neve call accept().
    so the connection stays waiting.
    so the next loop does:
    poll()
    ↓
    "Connection still waiting!"
    ↓
    poll()
    ↓
    "Connection STILL waiting!"
    ↓
    poll()
    ↓
    ...
    */

    
}
/*
                main.cpp
                    │
                    │ Server server(6667)
                    ▼
                Server
                    │
                    ▼
                start()
                    │
                    ├── socket()
                    │
                    ├── setsockopt()
                    │
                    ├── fcntl(O_NONBLOCK)
                    │
                    ├── sockaddr_in
                    │
                    ├── bind()
                    │
                    └── listen()
     server will exit after printing the last line.
    **En conclusion,** le serveur s’arrête volontairement après avoir affiché la dernière ligne, car nous n’avons pas encore ajouté `poll()`. À ce stade, il n’y a donc pas encore de boucle permettant au serveur de rester actif et d’attendre les requêtes.
*/

/*
when we run our programme, our program immediately returns to the shell. and that means our server is not actually staying alive yet.
Because start() reaches the end: 
                std::cout << "Server is listening on port "
                    << _port << std::endl; 
                finishes → destructor runs → socket closes → program exits.
                

                ➜  ft_irc git:(server) ✗ ./ircserv 
                Socket created!
                server_fd = 3
                SO_REUSEADDR enabled!
                Socket is non-blocking!
                Bind successful!
                Server is listening on port 6667
                ➜  ft_irc git:(server) ✗ 
    

*/

/*
soooo the next version should be become
                socket()
                ↓
                setsockopt()
                ↓
                fcntl()
                ↓
                bind()
                ↓
                listen()
                ↓
                ┌──────────────────────┐
                │        poll()        │
                │                      │
                │    WAIT FOR EVENT    │
                └──────────┬───────────┘
                        │
                        ▼
                    Something happened?
                        │
                    ┌────┴────┐
                    │         │
                    YES        NO
                    │         │
                    ▼         └──────► poll() again
                handle
                    │
                    ▼
                poll() again

And eventually: 
                poll()
                │
                ├── server_fd ready
                │       ↓
                │    accept()
                │       ↓
                │   new client
                │
                └── client_fd ready
                        ↓
                    recv()
                    
*/