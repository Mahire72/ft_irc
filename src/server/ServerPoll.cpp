/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerPoll.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 15:16:51 by mamaimai          #+#    #+#             */
/*   Updated: 2026/10/09 18:00:36 by mayoucha         ###   ########.fr       */
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


//enregistre un nouveau fd (socket) dans le tableau surveille par poll()
void Server::addPollFd(int fd)
{
    struct pollfd pfd; //tableau de structures --> chaque element represente un fd a surveiller

    pfd.fd = fd;
    pfd.events = POLLIN; //masque - previent qd un nouveau client tente de se connecter (_serverFd) ou si un client a envoye un msg
    pfd.revents = 0; //'returned events' - resultat des evenements lors de l'appel a poll()

    _pollFds.push_back(pfd);
}


//cette methode traite l'arrivee d'un nv client
void Server::handleNewConnection()
{
    //int clientFd = accept(_serverFd, NULL, NULL); //extrait la demande de connexion de la file d'attente du serveur

    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    int clientFd = accept(_serverFd,
                         (struct sockaddr *)&clientAddr,
                         &clientAddrLen);
    // IP
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

    if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1) //on passe la socket individuelle du client en non-bloquant
    {
        std::cerr << "fcntl() failed for client" << std::endl;
        close(clientFd);
        return;
    }

    addPollFd(clientFd); //ajoute le client au monitoring de poll()

    User newUser(clientFd); //creation de User
    newUser.setIP(clientIp);
    _users.insert(std::make_pair(clientFd, newUser)); //stockage dans la map (variable _users de Server)
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

    close(fd);   //fermeture de la socket
    _pollFds.erase(_pollFds.begin() + index); //retire l'element du vecteur _pollFds a l'index specifie

    _users.erase(fd); //on supprime le User de notre map
    std::cout << "User with fd " << fd << " removed." << std::endl;
}



//cette methode gere la lecture du reseau, la deconnexion propre et les erreurs
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

        User &user = _users[clientFd];  //recuperer le User associe a ce Fd
        //user.setIsInServer(true);
        user.addTextToBuffer(buffer);  //ajouter le texte brut au buffer de User

        // extrait et traite TOUTES les commandes completes disponibles (\r\n)
        std::string command;
        while(!(command = user.extractCommand()).empty())
        {
            std::cout << "[Client " << clientFd
                      << " Command]: "
                      << command << std::endl;
            // parseAndExecuteCommand(user, command);   /////TO CODE!!!!
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
            // if (commandName == "PASS")
            //     handlePass(user, arguments);
            // std::cout << "Command: " << commandName << std::endl;
            // std::cout << "Arguments: " << arguments << std::endl;

            // if (commandName == "NICK")
            //     handleNick(user, arguments);
            // std::cout << "Command: " << commandName << std::endl;
            // std::cout << "Arguments: " << arguments << std::endl;
            dispatchCommand(user, commandName, arguments);
        }


        /*std::cout << "Received from fd "
                  << clientFd
                  << ": "
                  << buffer;*/
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
        if (errno != EAGAIN && errno != EWOULDBLOCK) // car en mode non-bloquant, s'il n'y a rien a lire recv() renvoie -1 et met errno a EAGAIN
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
        int ret = poll(&_pollFds[0],
                       _pollFds.size(),
                       -1);            //pour eviter une surconsommation de CPU et permettre le blockage de poll() until new event

        if (ret == -1)
        {
            if (errno == EINTR)
            {
                if (g_stopServer)
                    break;
                continue;
            }
            std::cerr << "poll() failed" << std::endl;
            return;
        }

        for (size_t i = 0; i < _pollFds.size(); ++i)
        {
            if (_pollFds[i].revents & POLLIN)
            {
                if (i == 0)  //serveur principal _serverFd
                {
                    handleNewConnection();
                }
                else //cas d'un client deja connecte et qui envoie un texte
                {
                    size_t oldSize = _pollFds.size();

                    handleClient(i);

                    if (_pollFds.size() < oldSize) //pour eviter de sauter un client si removeClient a ete appele dans handleClient
                        --i;
                }
            }
            else if (_pollFds[i].revents & (POLLHUP | POLLERR)) //gestion de la fermeture soudaine / ou erreur reseau sur le socket
            {
                if (i != 0)
                {
                    removeClient(i); //on nettoie proprement le client et on ajuste l'index i
                    --i;
                }
            }
        }
    }
    std::cout << "Server poll loop stopped." << std::endl;
}
