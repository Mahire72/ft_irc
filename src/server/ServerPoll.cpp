/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerPoll.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 15:16:51 by mamaimai          #+#    #+#             */
/*   Updated: 2026/09/06 17:15:54 by mamaimai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/Server.hpp"

#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>

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
    int clientFd = accept(_serverFd, NULL, NULL);

    if (clientFd == -1)
    {
        std::cerr << "accept() failed" << std::endl;
        return;
    }

    std::cout << "New client connected! fd = "
              << clientFd << std::endl;

    if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
    {
        std::cerr << "fcntl() failed for client" << std::endl;
        close(clientFd);
        return;
    }

    addPollFd(clientFd);
    //User user(clientFd);
   // _users.push_back(user);
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
    close(_pollFds[index].fd);

    _pollFds.erase(_pollFds.begin() + index);
}

void Server::handleClient(size_t index)
{
    char buffer[1024];

    int bytes = recv(_pollFds[index].fd,
                     buffer,
                     sizeof(buffer) - 1,
                     0);

    if (bytes > 0)
    {
        buffer[bytes] = '\0';

        std::cout << "Received from fd "
                  << _pollFds[index].fd
                  << ": "
                  << buffer;
    }
    else if (bytes == 0)
    {
        std::cout << "Client fd "
                  << _pollFds[index].fd
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
    while (true)
    {
        int ret = poll(&_pollFds[0],
                       _pollFds.size(),
                       -1);

        if (ret == -1)
        {
            if (errno == EINTR)
                continue;

            std::cerr << "poll() failed" << std::endl;
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
}