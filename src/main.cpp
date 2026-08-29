/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 22:02:53 by mamaimai          #+#    #+#             */
/*   Updated: 2026/08/29 23:02:46 by mamaimai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>// std::cout
#include <sys/socket.h> // socket


int main()
{
    std::cout <<"Hello from my server!" << std::endl;

    int server_fd; // os give our program an identifer.
    // if server_fd = 3;  
    /*
        server_fd
            │
            ▼
        [ 3 ]
            │
            ▼
        Operating System
            │
            ▼
        TCP Socket
    our program uses 3 to refer to that socket.
    */
    
    server_fd = socket(AF_INET, SOCK_STREAM, 0); // Give me an IPV4 TCP socket
    // asked the OS to create a TCP socket for server"
    //AF_INET: ipv4
    //SOCK_STREAM :  a stream-oriented socket. in ft_irc means TCP.
    
    
    if (server_fd == -1)
    {
        std::cerr << "socket() failed" << std::endl;
        return 1;
    }
    std::cout <<"Socket created!" << std::endl;
    std::cout << "server_fd = " << server_fd << std::endl; 
    /*
    after socket(..) we have our program and TCP , We're not a server yet,and nobody knows where is this server?
    so, bind() will create a specified port
                socket()
                ↓
                bind()
                ↓
                listen()
                ↓
                accept()
    */
    return 0;
}