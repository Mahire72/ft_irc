/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 12:07:48 by mamaimai          #+#    #+#             */
/*   Updated: 2026/09/12 17:00:46 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "User.hpp"
#include "Channel.hpp"

#include <vector>
#include <poll.h>
#include <map>
#include <cstddef>
#include <string>
#include <cstdlib>
#include <unistd.h>


///MAKSA modifs :
// * added "std::map<int, User> _users;"  in class Server
// * modified handleNewConnection : added 2 lines to add User and to stock it in map
// * modified removeClient : added 2 libes to remove User from map
// * modified handleClient : voir fonction



class Server
{
private:
    int _serverFd;
    int _port;
    std::string _name = "ircserv";

    std::string _password;
        // Server
        // ├── port = 6667
        // └── password = "secret123"

    std::vector<struct pollfd> _pollFds;
    std::map<int, User> _users; // Associe chaque FD de client à son instance de User
    std::map<std::string, Channel> _channels; //Maksa - liste de tous les canaux du serveur


    // Network setup
    void setupSocket();
    void setupAddress();
    void startListening();

    // Poll / client handling
    void addPollFd(int fd);
    void run();
    void handleNewConnection();
    void handleClient(size_t index);
    void removeClient(size_t index);


    //password authentication
    void handlePass(User& user, const std::string& argumments);

    //for NICK
    void handleNick(User& user, const std::string& arguments);

    //for JOIN (Maksa)
    void executeJoin(User &sender, const std::vector<std::string> &args);


public:
    void sendResponse(int fd, const std::string &message); //Maksa
    Server(int port, const std::string& password);
    ~Server();

    void start();

    //getter
    std::string getName() const;

};

#endif
