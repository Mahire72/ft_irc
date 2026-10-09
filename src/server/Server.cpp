/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 15:15:28 by mamaimai          #+#    #+#             */
/*   Updated: 2026/10/09 18:05:32 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/Signal.hpp"

#include <ctime>

Server::Server(int port, const std::string &password)
    : _serverFd(-1), _port(port), _password(password)
{

}

void Server::handlePass(User& user, const std::string& arguments)
{
    if (arguments.empty())
    {
        sendResponse(user.getFD(), ":ircserv 461 * PASS :Not enough parameters\r\n");
        return;
    }

    if (user.getRegistered())
    {
        sendResponse(user.getFD(),
            "462 :Unauthorized command (already registered)\r\n");
        return;
    }

    if (arguments == _password)
    {
        user.setHasPass(true);
        user.setPASS(true); //TO CHECK
        checkRegistration(user); // Vérification à chaque étape
        return;
    }

    //otherwise
    sendResponse(user.getFD(), ":ircserv 464 * :Password incorrect\r\n");
    // if (arguments == _password)
    // {
    //     user.setHasPass(true);
    //     std::cout << "PASS correct! hasPass = "
    //               << user.hasPass() << std::endl;
    // }
    // else
    //     sendResponse(user.getFD(), ":ircserv 464 * :Password incorrect\r\n");
}
/*

             PASS
              │
              ↓
       Does it have argument?
          /          \
        NO            YES
        ↓              ↓
 ERR_NEEDMORE     Already registered?
                     /       \
                   YES         NO
                   ↓            ↓
          ERR_ALREADY     Compare password
          REGISTERED          /      \
                             OK       WRONG
                             ↓          ↓
                      hasPass=true   error
*/



Server::~Server()
{
    for (size_t i = 0; i < _pollFds.size(); ++i)
    {
        int fd = _pollFds[i].fd;

        if (fd != _serverFd)
        {
            std::string message = "ERROR :Server shutting down\r\n";
            send(fd, message.c_str(), message.size(), 0);
        }

        std::cout << "Closing fd " << _pollFds[i].fd << std::endl;
        close(_pollFds[i].fd);
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

std::string Server::getCurrentTime()
{
    std::time_t now = std::time(NULL);
    std::tm *timeInfo = std::localtime(&now);

    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%B %d %Y %H:%M:%S", timeInfo);

    return std::string(buffer);
}

/*
%B --full month name
%d --day of the month
%Y --year
*/

void Server::checkRegistration(User &user)
{
    // On ne ré-enregistre pas un utilisateur déjà validé
    if (user.getRegistered())
    {
        sendResponse(user.getFD(),
            "462 :Unauthorized command (already registered)\r\n");
        return  ;
    }

    // Condition stricte : Les 3 doivent être VRAIS
    if (user.getPASS() && user.getNICK() && user.getUSER())
    {
        user.setRegistered(true);

        std::string nick = user.getNickname();
        std::string host = this->getName();

        sendResponse(user.getFD(), ":" + host + " 001 " + nick + " :Welcome to the IRC Network " + nick + "\r\n");
        sendResponse(user.getFD(), ":" + host + " 002 " + nick + " :Your host is " + host + ", running version 1.0\r\n");
        sendResponse(user.getFD(), ":" + host + " 003 " + nick + " :This server is created " +getCurrentTime() + "\r\n");
        sendResponse(user.getFD(), ":" + host + " 004 " + nick + " " + host + " 1.0 itkol\r\n");
        }
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
        std::string err = ":" + this->getName() + " " + err_nb + " " + sender.getNickname() + " " + err_message + "\r\n";
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
    else
    {
        sendResponse(user.getFD(),
            ":ircserv 421 " + commandName +
            " :Unknown command\r\n");
    }
}
