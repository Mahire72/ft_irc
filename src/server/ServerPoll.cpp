/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerPoll.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 15:16:51 by mamaimai          #+#    #+#             */
/*   Updated: 2026/10/11 00:34:17 by mamaimai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/Server.hpp"
#include "../../include/Parser.hpp"
#include "../../include/Signal.hpp"

#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <netinet/in.h>
#include <arpa/inet.h>

void Server::addPollFd(int fd)
{
    struct pollfd pfd;

    pfd.fd = fd;
    pfd.events = POLLIN;
    pfd.revents = 0;

    _pollFds.push_back(pfd);
}

void Server::handleNewConnection()
{
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    int clientFd = accept(_serverFd,
                         (struct sockaddr *)&clientAddr,
                         &clientAddrLen);
    if (clientFd == -1)
    {
        std::cerr << "accept() failed" << std::endl;
        return;
    }

    std::string clientIp = inet_ntoa(clientAddr.sin_addr);

    std::cout << "New client connected! fd = "
              << clientFd
              << " IP = "
              << clientIp
              << std::endl;

    if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
    {
        std::cerr << "fcntl() failed for client" << std::endl;
        close(clientFd);
        return;
    }

    addPollFd(clientFd);

    User newUser(clientFd);
    newUser.setIP(clientIp);
    _users.insert(std::make_pair(clientFd, newUser));
}

/*
the connection produces two things:
    accept()
    │
    ├── clientFd → poll list
    │
    └── clientFd → User
*/



void Server::removeClient(size_t index)
{
    int fd = _pollFds[index].fd;

    close(fd);
    _pollFds.erase(_pollFds.begin() + index);

    _users.erase(fd);
    std::cout << "User with fd " << fd << " removed." << std::endl;
}

void Server::handleClient(size_t index)
{
    char buffer[1024 * 1024];
    int clientFd = _pollFds[index].fd;

    int bytes = recv(clientFd,
                     buffer,
                     sizeof(buffer) - 1,
                     0);

    if (bytes > 0)
    {
        buffer[bytes] = '\0';

        std::cout << "DEBUG received: [" << buffer << "]" << std::endl;

        for (int i = 0; i < bytes; ++i)
        {
            std::cout << "byte " << i
                      << " = " << (int)buffer[i] << std::endl;
        }

        User &user = _users[clientFd];
        user.addTextToBuffer(buffer);
        
        std::string command;
        while(!(command = user.extractCommand()).empty())
        {
            std::cout << "[Client " << clientFd
                      << " Command]: "
                      << command << std::endl;
                      
            std::string commandName = Parser::getCommand(command);
            std::string arguments = Parser::getArguments(command);

            std::cout << "COMMAND = [" << commandName << "]" << std::endl;
            std::cout <<"ARGUMENTS [" << arguments << "]" << std::endl;

            std::vector<std::string> parameters = Parser::getParameters(arguments);
            for (std::size_t i = 0; i < parameters.size(); ++i)
            {
                std::cout << "PARAM[" << i << "] = ["
                          << parameters[i] << "]"
                          << std::endl;
            }
            dispatchCommand(user, commandName, arguments);
        }
    }
    else if (bytes == 0)
    {
        std::cout << "Client fd "
                  << clientFd
                  << " disconnected"
                  << std::endl;

        removeClient(index);
    }
    else
    {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
        {
            std::cerr << "recv() failed" << std::endl;
            removeClient(index);
        }
    }
}

void Server::run()
{
    while (!g_stopServer)
    {
        int resultat_retourne = poll(&_pollFds[0],
                       _pollFds.size(),
                       -1);

        if (resultat_retourne == -1)
        {
            if (errno == EINTR)
            {
                if (g_stopServer)
                    break;
                continue;
            }
            std::cerr << "poll() failed " << std::strerror(errno) << std::endl;
            return;
        }

        for (size_t i = 0; i < _pollFds.size(); ++i)
        {
            if (_pollFds[i].revents & POLLIN)
            {
                if (i == 0)
                {
                    handleNewConnection();
                }
                else
                {
                    size_t oldSize = _pollFds.size();

                    handleClient(i);

                    if (_pollFds.size() < oldSize)
                        --i;
                }
            }
            else if (_pollFds[i].revents & (POLLHUP | POLLERR))
            {
                if (i != 0)
                {
                    removeClient(i);
                    --i;
                }
            }
        }
    }
    std::cout << "Server poll loop stopped." << std::endl;
}
