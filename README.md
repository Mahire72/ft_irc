# Contributors
This project has been created as part of the 42 curriculum by mamaimai, mayoucha, omawele.

# Description
**ft_irc** is a 42 School project focused on implementing an IRC (Internet Relay Chat) server in C++98. The goal is to build a functional TCP server that follows the core IRC protocol and allows multiple clients to connect, register, communicate privately, and exchange messages through channels.

The server uses **non-blocking I/O and a single `poll()` event loop** to monitor the listening socket and all connected client sockets. This architecture allows it to handle multiple clients simultaneously without blocking while waiting for a particular client.

## Key Features

TCP networking
                Create a server, listen on a specified port, and accept incoming client connections.

`Non-blocking I/O`
    Use non-blocking sockets and `poll()` to monitor multiple connections through a single event loop.
    
`User registration`
    Authenticate clients using a server password and manage nicknames and usernames through `PASS`, `USER`, `NICK`.

`Channel management`
    Allow users to join channels, manage topics, and handle channel membership and permissions.

`Messaging`
    Support private messages between users and messages sent to channels using `PRIVMSG`.

`IRC commands`
    Implement commands such as `JOIN`, `KICK`, `INVITE`, `TOPIC`, and `MODE`.

`Channel and user modes`
    Manage channel permissions and restrictions, including supported modes such as `+i`, `+t`, `+k`, `+o`, and `+l`.

`Protocol compliance and error handling`
    Parse IRC messages, return appropriate numeric replies, and handle invalid commands and client disconnections.

`Connection lifecycle`
    Manage client connections and ensure proper cleanup during disconnections and server shutdown.

## Learning Objectives

    This project provides hands-on experience with TCP/IP networking, socket programming, non-blocking I/O, event-driven architecture, protocol implementation, and multi-client connection management. It also develops a deeper understanding of how network applications communicate with the operating system and manage data over TCP connections.

# Instructions

## Prerequisites
A C++ compiler compatible with **C++98** (such as `c++`).

`make` to compile the project.
 An IRC client such as `Irssi`, `Netcat`(`nc`)**, or another compatible IRC client for testing(`WEECHAT, Hexchat).

## Compilation
Clone the repository and navigate to the project directory:

                Open the terminal.
                git clone <repository-url>
                cd ft_irc
Compile the project using the Makefile:
                make

Additional Makefile commands:

                make clean   # Remove object files
                make fclean  # Remove object files and the executable
                make re      # Recompile the project from scratch

## Execution

Run the IRC server with a port number and a connection password:
                ./ircserv <port> <password>
For example:
                ./ircserv 6667 secret

- `<port>`: The TCP port on which the server listens for incoming connections.
- `<password>`: The password clients must provide to register with the server.

Make sure the selected port is available and not already in use.

## Connecting to the server

You can connect using Netcat:open second Terminal.

                nc 127.0.0.1 6667 (linux)
                nc -c 127.0.0.1 6667 (for mac)
Once connected, send the IRC registration commands:

                PASS secret
                NICK alice
                USER alice 0 * :Alice

After successful registration, the server should send the IRC welcome replies.

You can then join a channel:
                JOIN #42

And send a message to the channel:

                PRIVMSG #42 :Hello everyone!

To test communication between multiple clients, open another terminal and connect to the same server using the same port and password, but choose a different nickname.

## Supported IRC Commands

The server implements IRC commands for user registration, channel management, messaging, and operator permissions.

### User Registration

Command         Description                                                          Example

`PASS`          Provides the password required to register with the server.         `PASS secret`
`NICK`          Sets or changes the user's nickname.                                `NICK alice`
`USER`          Provides the username and real name for registration.               `USER alice 0 * :Alice Smith`

### Channel Management

Command         Description                                                           Example

`JOIN`          Joins an existing channel or creates it if permitted by              `JOIN #42`
                the server. 
`TOPIC`         Displays or changes the topic of a channel, subject to               `TOPIC #42 :Welcome everyone!`
                channel permissions.

`KICK`          Removes a user from a channel. Requires channel operator             `KICK #42 bob :Rule violation`
                privileges. 

`INVITE`        Invites a user to join a channel.                                    `INVITE bob #42`

`MODE`          Displays or modifies user or channel modes, depending                `MODE #42 +t`
                on the supported implementation. 

### Messaging
Command         Description                                                           Example

`PRIVMSG`       Sends a private message to a user or a message to a channel.         `PRIVMSG bob :Hello!`

`PRIVMSG`       Sends a message to all eligible members of a channel.                `PRIVMSG #42 :Hello everyone!`

### Channel Operator Permissions

A channel operator can perform privileged actions, such as kicking users and modifying restricted channel settings, according to the supported modes and permissions.

For example:

                MODE #42 +o alice   
                MODE #42 +t
                KICK #42 bob :Goodbye!
                TOPIC #42 :New channel topic

- `+o` grants channel operator status to a user.
- `+t` restricts who can change the channel topic, typically to channel operators.

**Note:** The available commands and modes depend on the features implemented by the server. Check the project implementation for the exact supported behavior.

### Stopping the server

Press `Ctrl+C` in the server terminal to stop the server. The server should exit its event loop and close its listening socket and connected client sockets.


# Resources

https://www.rfc-editor.org/info/rfc2812/

https://www.geeksforgeeks.org/computer-networks/socket-in-computer-network/

https://www.ibm.com/docs/en/i/7.4.0?topic=programming-how-sockets-work

https://www.ibm.com/docs/fr/i/7.6.0?topic=designs-using-poll-instead-select

https://www.ibm.com/docs/fr/i/7.6.0?topic=p-wait-events-multiple-descriptors-poll
