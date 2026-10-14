/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerNetwork.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 15:15:58 by mamaimai          #+#    #+#             */
/*   Updated: 2026/10/14 12:46:14 by mamaimai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"

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
    if (listen(_serverFd, 42) == -1)
    {
        std::cerr << "listen() failed" << std::endl;
        return;
    }
    std::cout << "Server is listening on port "
              << _port << std::endl;
}


void Server::sendResponse(int destFd, const std::string &message)
{
    bool found = false;

    for (size_t i = 0; i < _pollFds.size(); ++i)
    {
        if (_pollFds[i].fd == destFd)
        {
            found = true;
            break;
        }
    }

    if (!found)
        return;

    std::string message_copy = message;

    size_t len = message_copy.length();

    if ((len < 2) || (message_copy[len - 2] != '\r' || message_copy[len - 1] != '\n'))
        message_copy += "\r\n";

    _outputBuffers[destFd] += message_copy;

    size_t i = 0;

    while (i < _pollFds.size())
    {
        if (_pollFds[i].fd == destFd)
        {
            _pollFds[i].events = _pollFds[i].events | POLLOUT;
            break;
        }
        i++;
    }
}
