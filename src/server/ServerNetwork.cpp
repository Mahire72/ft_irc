/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerNetwork.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 15:15:58 by mamaimai          #+#    #+#             */
/*   Updated: 2026/09/07 13:16:29 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

// This file's responsibility is: Get the server socket ready to accept connections

void Server::setupSocket()
{
    _serverFd = socket(AF_INET, SOCK_STREAM, 0); //demande au SE une socket de type IPv4 (AF_INET) et TCP (SOCK_STREAM)

    if (_serverFd == -1)
    {
        std::cerr << "socket() failed" << std::endl;
        return;
    }

    std::cout << "Socket created!" << std::endl;
    std::cout << "server_fd = " << _serverFd << std::endl;

    int opt = 1;

    if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR,
                   &opt, sizeof(opt)) == -1)            //evite le freez du serveur entre 2 redemarrages
    {
        std::cerr << "setsockopt() failed" << std::endl;
        return;
    }

    std::cout << "SO_REUSEADDR enabled!" << std::endl;

    if (fcntl(_serverFd, F_SETFL, O_NONBLOCK) == -1)  //condition de non-block pour eviter le freeze du serveur qd il n'y a rien a lire (couple avec poll() )
    {
        std::cerr << "fcntl() failed" << std::endl;
        return;
    }

    std::cout << "Socket is non-blocking!" << std::endl;
}


// cette methode prepare et associe l'adresse reseau / port a notre socket
void Server::setupAddress()
{
    struct sockaddr_in address;

    address.sin_family = AF_INET;  //protocole IPv4
    address.sin_port = htons(_port); //Host to Network Short - convertit le numero de port au format d'octets reseau
    address.sin_addr.s_addr = htonl(INADDR_ANY);  //Host to Network Long - ndique au serveur d'écouter sur toutes les interfaces réseau disponibles de la machine

    //assigne IP et le port configure au _serverFd
    if (bind(_serverFd,
             (struct sockaddr *)&address,
             sizeof(address)) == -1)     //on cast car bind() attend un ptr vers sockaddr
    {
        std::cerr << "bind() failed" << std::endl;
        return;
    }

    std::cout << "Bind successful!" << std::endl;
}


//met la socket en mode ecoute
void Server::startListening()
{
    if (listen(_serverFd, 10) == -1) //10 represente la taille de la file d'attente des connexions non-traitees
    {
        std::cerr << "listen() failed" << std::endl;
        return;
    }

    std::cout << "Server is listening on port "
              << _port << std::endl;
}


void Server::sendResponse(int destFd, const std::string &message)
{
    std::string message_copy = message;

    size_t len = message_copy.length();
    if ((len < 2) || (message_copy[len - 2] != '\r' || message_copy[len - 1] != '\n'))
        message_copy += "\r\n";
    if(send(destFd, message_copy.c_str(), message_copy.length(), 0) == -1)
    {
        std::cerr << "send() failed" << std::endl;
    }
}


//fd = socket du client qui doit recevoir le msg
// message = le texte a lui envoyer









//ssize_t send(int s, const void *buf, size_t len, int flags); -- permet de transmettre un message a destination d'une autre socket

//s = fd de la socket emettrice
//buf = message a trasnmettre
//len = longueur du msg
// flags ???

