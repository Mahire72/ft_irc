/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 22:02:53 by mamaimai          #+#    #+#             */
/*   Updated: 2026/08/31 12:09:55 by mamaimai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <iostream>// std::cout
// #include <sys/socket.h> // socket, bind
// #include <netinet/in.h>
// #include <unistd.h>



// int main()
// {
//     std::cout <<"Hello from my server!" << std::endl;

//     int server_fd; // os give our program an identifer.
//     // if server_fd = 3;  
//     /*
//         server_fd
//             │
//             ▼
//         [ 3 ]
//             │
//             ▼
//         Operating System
//             │
//             ▼
//         TCP Socket
//     our program uses 3 to refer to that socket.
//     */
    
//     server_fd = socket(AF_INET, SOCK_STREAM, 0); // Give me an IPV4 TCP socket
//     // asked the OS to create a TCP socket for server"
//     //AF_INET: ipv4
//     //SOCK_STREAM :  a stream-oriented socket. in ft_irc means TCP.
    
    
//     if (server_fd == -1)
//     {
//         std::cerr << "socket() failed" << std::endl;
//         return 1;
//     }
//     std::cout <<"Socket created!" << std::endl;
//     std::cout << "server_fd = " << server_fd << std::endl; 
//    /*
//     After socket(), we have created a socket, but we are not
//     a server yet. Nobody knows which address and port this
//     socket should use.

//     We want to tell the operating system:
//     "This socket belongs to this address and port."

//     bind() assigns an address and port to our socket.

//     Think of it like a telephone:

//         socket()  → give me a telephone
//             ↓
//         bind()    → give this telephone a phone number
//             ↓
//         listen()  → wait for people to call
//             ↓
//         accept()  → someone is calling; accept the call
// */

// /*
//     Complete story:

//         socket()
//             │
//             │ Create a communication endpoint
//             ▼
//         bind()
//             │
//             │ Assign an IP address and port
//             ▼
//         listen()
//             │
//             │ Wait for incoming connections
//             ▼
//         accept()
//             │
//             │ Accept a client connection
//             ▼
//         Client connected
// */

// /*
//     bind() needs an address structure.

//     For IPv4, we use:
//         struct sockaddr_in.
//     rest of explication bind() at the bind().readme.
// */
//     struct sockaddr_in adress;
//     adress.sin_family = AF_INET;
//     adress.sin_port = htons(6667);
//     adress.sin_addr.s_addr = htonl(INADDR_ANY);
    
//     if (bind(server_fd,
//             (struct sockaddr *)&adress,
//             sizeof(adress)) == -1)
//     {
//         std::cerr << "bind() failed" << std::endl;
//         close(server_fd);
//         return 1;
//     }
    
//     std::cout << "Bind successful!" << std::endl;
// /*
//                     OUR SERVER

//                 socket()
//                 │
//                 │ "Give me a TCP socket"
//                 ▼
//                 server_fd
//                 │
//                 ▼
//                 sockaddr_in
//                 │
//                 ├── sin_family → IPv4
//                 ├── sin_addr   → IP address
//                 └── sin_port   → 6667
//                 │
//                 ▼
//                 bind()
//                 │
//                 │ "Associate this socket with this address"
//                 ▼
//                 server socket bound to port 6667
// */

// // next step will be listen()
//     if (listen(server_fd, 10) == -1) // backlog is 10 for just testing
//     {
//         std::cerr << "listen() failled" << std::endl;
//         close(server_fd);
//         return 1;
//     }
//     std::cout <<"Server is listenning!" << std::endl;

// // next step is accept()
//     int client_fd = accept(server_fd, NULL, NULL);

//     if (client_fd == -1)
//     {
//         std::cerr << "accept() failed" << std::endl;
//         close(server_fd);
//         return 1;
//     }

//     std::cout << "Client connected!" << std::endl;
//     std::cout << "client_fd = " << client_fd << std::endl;
        
//     close(server_fd);
//     return 0;
// }


#include "../include/Server.hpp"

int main()
{
    Server server(6667);

    server.start();

    return 0;
}