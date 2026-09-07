/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 12:07:48 by mamaimai          #+#    #+#             */
/*   Updated: 2026/09/06 19:12:00 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "User.hpp"

#include <vector>
#include <poll.h>
#include <map>
#include <cstddef>


///MAKSA modifs :
// * added "std::map<int, User> _users;"  in class Server (on private)
// * modified handleNewConnection : added 2 lines to add User and to stock it in map
// * modified removeClient : added 2 libes to remove User from map
// * modified handleClient : voir fonction



class Server
{
private:
    int _serverFd;
    int _port;
    private:

    std::vector<struct pollfd> _pollFds;
    std::map<int, User> _users; // Associe chaque FD de client à son instance de User


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

public:
    Server(int port);
    ~Server();

    void start();
};

#endif
