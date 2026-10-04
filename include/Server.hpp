/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 12:07:48 by mamaimai          #+#    #+#             */
/*   Updated: 2026/10/04 19:40:01 by mayoucha         ###   ########.fr       */
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
#include <sstream>


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
    std::string _name;
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


    ////COMMANDES
    void handlePass(User& user, const std::string& argumments);
    void handleNick(User& user, const std::string& arguments);
    void handleUser(User &user, const std::string &arguments);
    void handlePrivmsg(User &sender, const std::string &arguments);

    //for JOIN
    void joinSingleChannel(User &sender,
    const std::string &channelName, const std::string &key);
    std::vector<std::string> splitByComma(const std::string &str);
    void leaveAllChannels(User &sender);
    void handleJoin(User &sender, const std::string &arguments);


    //for MODE
    void handleMode(User &sender, const std::string &arguments);
    void displayChannelModes(User &sender, Channel &channel);

    std::string applyModes(User &sender, Channel &channel,
    const std::vector<std::string> &args);

    std::string applyModeKey(User &sender, Channel &channel,
    const std::vector<std::string> &args, size_t &paramIndex, bool adding);

    std::string applyModeLimit(User &sender, Channel &channel,
    const std::vector<std::string> &args, size_t &paramIndex, bool adding);

    std::string applyModeOperator(User &sender, Channel &channel,
    const std::vector<std::string> &args, size_t &paramIndex, bool adding);


    //for Error messages
    void sendError(const std::string &err_nb, const std::string &err_message, User &sender);
    bool isValidNickname(const std::string& nickname);
    bool isNicknameTaken(const std::string& nickname);

    //421 ERR_UNKNOWNCOMMAND for a command unknown to the server.
    //dispatcher declaration

    void dispatchCommand(User& user,
                        const std::string& commandName,
                        const std::string& arguments);

public:
    void sendResponse(int fd, const std::string &message); //Maksa
    Server(int port, const std::string& password);

    ~Server();

    void start();

    //getter
    std::string getName() const;

};

#endif
