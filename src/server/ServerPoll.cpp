/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerPoll.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 15:16:51 by mamaimai          #+#    #+#             */
/*   Updated: 2026/10/12 12:09:17 by mamaimai         ###   ########.fr       */
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
    if (clientFd < 0)
    {
        std::cerr << "accept() failed" << std::endl;
        return;
    }
    

    if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
    {
        std::cerr << "fcntl() failed for client" << std::endl;
        close(clientFd);
        return;
    }

    std::string clientIp = inet_ntoa(clientAddr.sin_addr);

    addPollFd(clientFd);

    User newUser(clientFd);
    newUser.setIP(clientIp);
    _users.insert(std::make_pair(clientFd, newUser));
    
    std::cout << "New client connected! fd = "
              << clientFd
              << " IP = "
              << clientIp
              << std::endl; 
}

void Server::removeClient(size_t index)
{
    int fd = _pollFds[index].fd;

    close(fd);
    _pollFds.erase(_pollFds.begin() + index);

    _users.erase(fd);
    _outputBuffers.erase(fd);
    
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
        std::cerr << "recv() failed for fd " << clientFd << std::endl;
        removeClient(index);
    }
}

void Server::run()
{
    while (!g_stopServer)
    {
        if (_pollFds.empty())
            break;

        int resultat_retourne = poll(
            &_pollFds[0],
            _pollFds.size(),
            -1
        );

        if (resultat_retourne == -1)
        {
            if (errno == EINTR)
            {
                if (g_stopServer)
                    break;
                continue;
            }

            std::cerr << "poll() failed: "
                      << strerror(errno) << std::endl;
            break;
        }

        for (size_t i = 0; i < _pollFds.size(); )
        {
            int fd = _pollFds[i].fd;
            short events = _pollFds[i].revents;

            if (events == 0)
            {
                ++i;
                continue;
            }

            if (fd == _serverFd)
            {
                if (events & POLLIN)
                    handleNewConnection();

                if (events & (POLLERR | POLLHUP | POLLNVAL))
                {
                    std::cerr << "Server socket error" << std::endl;
                    g_stopServer = true;
                    break;
                }

                ++i;
                continue;
            }

            if (events & (POLLERR | POLLHUP | POLLNVAL))
            {
                removeClient(i);
                continue;
            }

            if (events & POLLOUT)
            {
                size_t oldSize = _pollFds.size();

                handleClientWrite(i);

                if (_pollFds.size() < oldSize)
                    continue;
            }

            if (i >= _pollFds.size())
                break;

            if (_pollFds[i].fd != fd)
                continue;

            if (events & POLLIN)
            {
                size_t oldSize = _pollFds.size();

                handleClient(i);

                if (_pollFds.size() < oldSize)
                    continue;
            }

            ++i;
        }
    }

    std::cout << "Server poll loop stopped." << std::endl;
}


void Server::handleClientWrite(size_t index)
{
    int fd = _pollFds[index].fd;
    
    std::map<int, std::string>::iterator it = _outputBuffers.find(fd);
    
    //std::string &output = _outputBuffers[fd];
    
    if (it == _outputBuffers.end() || it->second.empty())
    {
        _pollFds[index].events &= ~POLLOUT;
        if (it != _outputBuffers.end())
            _outputBuffers.erase(it);
        return;
    }

    std::string &output = it->second;
    ssize_t bytesSent = send(fd, output.c_str(), output.size(), 0);

    if (bytesSent > 0)
        output.erase(0, static_cast<size_t>(bytesSent));
    
    else
    {
        std::cerr << "send() failed for fd " << fd << std::endl;
        removeClient(index);
        return;
    }
    if (output.empty())
    {
        _pollFds[index].events &= ~POLLOUT;
        _outputBuffers.erase(fd);
    }
}