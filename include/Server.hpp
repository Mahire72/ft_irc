/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 12:07:48 by mamaimai          #+#    #+#             */
/*   Updated: 2026/10/14 12:46:24 by mamaimai         ###   ########.fr       */
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
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <ctime>



class Server
{
private:
    int         _serverFd;
    int         _port;
    std::string _name;
    std::string _password;

    std::vector<struct pollfd>      _pollFds;
    std::map<int, User>             _users;
    std::map<std::string, Channel>  _channels;
    std::map<int, std::string>      _outputBuffers; 

    void setupSocket();
    void setupAddress();
    void startListening();

    void addPollFd(int fd);
    void run();
    void handleNewConnection();
    void handleClient(size_t index);
    void removeClient(size_t index);
    void handleClientWrite(size_t index);

    void handlePass(User& user, const std::string &argumments);
    void handleNick(User& user, const std::string &arguments);
    void handleUser(User &user, const std::string &arguments);
    void handleTopic(User &sender, const std::string &arguments);
    void handlePrivmsg(User &sender, const std::string &arguments);
    void handleJoin(User &sender, const std::string &arguments);
    void handleMode(User &sender, const std::string &arguments);
    void handleKick(User &sender, const std::string &arguments);

    void joinSingleChannel(User &sender,
    const std::string &channelName, const std::string &key);
    std::vector<std::string> splitByComma(const std::string &str);
    void leaveAllChannels(User &sender);

    void displayChannelModes(User &sender, Channel &channel);
    std::string applyModes(User &sender, Channel &channel,
    const std::vector<std::string> &args);
    std::string applyModeKey(User &sender, Channel &channel,
    const std::vector<std::string> &args, size_t &paramIndex, bool adding);
    std::string applyModeLimit(User &sender, Channel &channel,
    const std::vector<std::string> &args, size_t &paramIndex, bool adding);
    std::string applyModeOperator(User &sender, Channel &channel,
    const std::vector<std::string> &args, size_t &paramIndex, bool adding);

    void handleInvite(User &sender, const std::string &arguments);

    void handlePing(User &sender, const std::string &arguments);
    void handlePong(User &sender, const std::string &arguments);

    void handleCap(User &sender, const std::string &arguments);

    void handleWho(User &sender, const std::vector<std::string> &arguments);

    Channel *findChannel(const std::string &ChannelName);
    User *findUserByNickname(const std::string &nickname);
    void sendKickNeedMoreParams(User &sender);
    void sendKickNoSuchChannel(User &sender, const std::string &channelName);
    void sendKickNotOnChannel(User &sender, const std::string &channelName);
    void sendKickNotOperator(User &sender, const std::string &channelName);
    void sendKickNoSuchNick(User &sender, const std::string &nickname);
    void sendKickUserNotInChannel(User &sender,
                              const std::string &nickname,
                              const std::string &channelName);

    void dispatchCommand(User &user,
                        const std::string &commandName,
                        const std::string &arguments);

    void sendError(const std::string &err_nb,
    const std::string &err_message, User &sender);
    bool isValidNickname(const std::string& nickname);
    bool isNicknameTaken(const std::string& nickname);

public:

    Server(int port, const std::string& password);
    ~Server();
    void sendResponse(int fd, const std::string &message);
    void checkRegistration(User &user);
    void start();

    std::string getName() const;
    std::string getCurrentTime();
    bool isUserInServer(std::string userNick) const;
    int getUserFd(std::string userNick) const;
    int getPort() const;
};

#endif
