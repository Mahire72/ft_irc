/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 15:15:28 by mamaimai          #+#    #+#             */
/*   Updated: 2026/10/14 12:46:20 by mamaimai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/Signal.hpp"

Server::Server(int port, const std::string &password)
    : _serverFd(-1), _port(port), _password(password) {}

int Server::getPort() const
{
    return _port;
}

Server::~Server()
{
    for (size_t i = 0; i < _pollFds.size(); ++i)
    {
        int fd = _pollFds[i].fd;

        std::cout << "Closing fd " << fd << std::endl;
        close(fd);
    }
    _pollFds.clear();
    _users.clear();
    _channels.clear();
}

void Server::start()
{
    signal(SIGINT, handleSignal);
    signal(SIGTERM, handleSignal);

    setupSocket();
    setupAddress();
    startListening();

    addPollFd(_serverFd);

    run();
}

std::string Server::getName() const
{
    return("ircserv");
}

bool Server::isUserInServer(std::string userNick) const
{
    for (std::map<int, User>::const_iterator it = _users.begin(); it != _users.end(); ++it)
    {
        if (it->second.getNickname() == userNick)
            return (true);
    }
    return(false);
}

int Server::getUserFd(std::string userNick) const
{
    for (std::map<int, User>::const_iterator it = _users.begin(); it != _users.end(); ++it)
    {
        if (it->second.getNickname() == userNick)
            return (it->first);
    }
    return(0);
}

void Server::sendError(const std::string &err_nb, const std::string &err_message, User &sender)
{
        std::string err = ":" + this->getName() + " " + err_nb + " " +
        sender.getNickname() + " " + err_message + "\r\n";
		this->sendResponse(sender.getFD(), err);
		return ;
}

void Server::dispatchCommand(User& user, const std::string& commandName,
                            const std::string &arguments)
{
    if (commandName == "PASS")
        handlePass(user, arguments);
    else if (commandName == "NICK")
        handleNick(user, arguments);
    else if (commandName == "USER")
        handleUser(user, arguments);
    else if (commandName == "JOIN")
        handleJoin(user, arguments);
    else if (commandName == "PRIVMSG")
        handlePrivmsg(user, arguments);
    else if (commandName == "MODE")
        handleMode(user, arguments);
    else if (commandName == "INVITE")
        handleInvite(user, arguments);
    else if (commandName == "TOPIC")
        handleTopic(user, arguments);
    else if (commandName == "KICK")
        handleKick(user, arguments);
    else if (commandName == "PING")
        handlePing(user, arguments);
    else if (commandName.size() >= 3 && commandName.compare(0, 3, "CAP") == 0)
        handleCap(user, arguments);
    else if (commandName == "PONG")
        handlePong(user, arguments);
    else
    {
        sendResponse(user.getFD(),
            ":ircserv 421 " + commandName + " "
            + commandName + " :Unknown command\r\n");
    }
}
