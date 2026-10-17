# Contributors
*This project has been created as part of the 42 curriculum by mamaimai, mayoucha, omawele.*

# Description
**ft_irc** is a 42 School project focused on implementing an IRC (Internet Relay Chat) server in C++98. The goal is to build a functional TCP server that follows the core IRC protocol and allows multiple clients to connect, register, communicate privately, and exchange messages through channels.

The server uses **non-blocking I/O and a single `poll()` event loop** to monitor the listening socket and all connected client sockets. This architecture allows it to handle multiple clients simultaneously without blocking while waiting for a particular client.

## Key Features

`TCP networking`
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

    This project provides hands-on experience with TCP/IP networking, socket programming, non-blocking I/O, 
    event-driven architecture, protocol implementation, and multi-client connection management. It also 
    develops a deeper understanding of how network applications communicate with the operating system 
    and manage data over TCP connections.

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
                make         # compile the project
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

                nc localhost port (linux)
                nc -c localhost port (for mac)
ex:
                nc 127.0.0.1 6667
Once connected, send the IRC registration commands:

                PASS secret
                NICK nickname
                USER username 0 * :realname

After successful registration, the server should send the IRC welcome replies.

You can then join a channel:
                JOIN #42

And send a message to the channel:

                PRIVMSG #42 :Hello everyone!

To test communication between multiple clients, open another terminal and connect to the same server using the same port and password, but choose a different nickname.

## Supported IRC Commands

### 1. User Registration

| Command | Description | Example |
|---|---|---|
| `PASS` | Provides the password required to register with the server. | `PASS secret` |
| `NICK` | Sets or changes the user's nickname. | `NICK alice` |
| `USER` | Provides the username and real name for registration. | `USER alice 0 * :Alice Smith` |

### 2. Channel Management

| Command | Description | Example |
|---|---|---|
| `JOIN` | Joins a channel or creates it if permitted by the server. It allows a user to join one or multiple channels. It supports passwords (+k), multi-channel joins using commas, and leaving all channels using JOIN 0. | `JOIN #42` |
| `TOPIC` | Displays or changes a channel's topic, subject to permissions. | `TOPIC #42 :Welcome everyone!` |
| `KICK` | Removes a user from a channel. Requires channel operator privileges. | `KICK #42 bob :Rule violation` |
| `INVITE` | Invites a user to join a channel. Invites a user to join a channel. If the channel is in invite-only mode (+i), this command allows the specified user to bypass the restriction and join successfully. | `INVITE bob #42` |
| `MODE` | Displays or modifies user or channel modes. Changes or views the modes of a channel. It supports flags such as invite-only (i), topic restrictions (t), key/password (k), user limit (l), and channel operator privileges (o).| `MODE #42 +t` |


### 3. Messaging

| Command | Description | Example |
|---|---|---|
| `PRIVMSG` | Sends a private message to another user. | `PRIVMSG bob :Hello!` |
| `PRIVMSG` | Sends a message to a channel's eligible members. | `PRIVMSG #42 :Hello everyone!` |

### 4. Channel Operator Permissions

Channel operators can perform privileged actions depending on the channel's modes and the permissions implemented by the server.

Examples:

```irc
MODE #42 +o alice
MODE #42 +t
KICK #42 bob :Rule violation
TOPIC #42 :Welcome everyone!
```

- `+o` — Grants channel operator status to a user.
- `+t` — Restricts who can change the channel topic, typically to channel operators.



### 5. Partial commands tests

`PASS`  *(with whitespace at the end)* then **`Ctrl+D`**
`sec` then **`Ctrl+D`**
`ret` then **Enter**

**It should be:**
`[Client 4 Command]: PASS secret`
`COMMAND = [PASS]`
`ARGUMENTS = [secret]`


### 6. Notes

- Commands and their parameters must follow the IRC protocol syntax.
- Some commands require channel membership or operator privileges.
- Available commands and modes depend on the features implemented by the server.

### Stopping the server

Press `Ctrl+C` in the server terminal to stop the server. The server should exit its event loop and close its listening socket and connected client sockets.

## Bonus: File Transfer and IRC Bot

### File Transfer

    The server supports file transfers between users using the Direct Client-to-Client (DCC) protocol.
    You can test this feature directly with IRC-compatible clients or Netcat (nc) by `sending` a file with 
                    /dcc send <nickname> <filename>
    and `accepting` incoming files with 
                   /dcc get <nickname>

### IRC Bot

As part of the bonus features, a dedicated IRC bot is included in this project.
The bot is simply a user (a client). We didn't hardcode it into the server's source code to preserve the logic, because an IRC server's only job is to manage network connections and route messages."

### How to use the bot
Compile the bot using the bonus rule:
                make bonus
Start the server in your first terminal:
                ./ircserv <port> <password>
Start the bot in a second terminal:
                ./bot <localhost> <port> <password>
Connect as a normal user (via Irssi or nc) in a third terminal and send a private message to the bot:
                PRIVMSG ft_bot :!ping
   *(The bot will instantly intercept the message and reply with a `pong!`)*


### Helpful commands for IRSSI

`bash-5.3$  valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./ircserv <port> <password>  to test leak 
open second terminal and tape irssi then enter irssi interface

                /connect localhost <port> <password> : to connect to the server
                /quit : to disconnect from the client server
                /set nick <nickname> : new nickname
                /window close : to close a private message or a channel window
                /query <username> or <channel> : to open a new private message with a user/channel
                /names : users present in a channel
                /window list : windows list
                /window <window number> : to navigate through windows



### Nickname & User Management
                /nick <new_nick> - Instantly change your nickname.
                /set nick <new_nick> - Set the default nickname.
                /set user_name <username> - Set the username (takes effect upon reconnecting).
                /set real_name <realname> - Set the real name / description.
                /save - Save settings to the Irssi configuration file.


## Channel & User Modes

The `/mode` command sets or unsets channel and user permissions (`+` enables, `-` disables).

### Common Channel Modes (ITKOL Mnemonic)
                /mode #channel +i  **Invite-only**: Users can only join if invited via `/invite`.
                /mode #channel +t  **Topic protection**: Only channel operators can change `/topic`.
                /mode #channel +k <password> -**Key**: Protects the channel with a password.
                /mode #channel +o <nick>  **Operator**: Grants channel operator privileges to a user.
                /mode #channel +l <limit>  **Limit**: Sets the maximum number of users allowed in the channel.

### Irssi Command Cheat Sheet
                /mode <#channel|nick> <flags> - Modify channel or user modes.
                /join <#channel> [key] - Join a channel.
                /invite <nick> - Invite a user to the current channel.
                /topic <new topic> - Set channel topic.
                /kick <nick> [reason] - Kick a user from the channel.
                /msg <target> <message> - Send a direct message (PRIVMSG).
                /nick <new_nick> - Change nickname.
                /quote PASS <password> - Send connection password to the IRC server.

# Resources

https://www.rfc-editor.org/info/rfc2812/

https://www.geeksforgeeks.org/computer-networks/socket-in-computer-network/

https://www.ibm.com/docs/en/i/7.4.0?topic=programming-how-sockets-work

https://www.ibm.com/docs/fr/i/7.6.0?topic=designs-using-poll-instead-select

https://www.ibm.com/docs/fr/i/7.6.0?topic=p-wait-events-multiple-descriptors-poll

https://www.mirc.com/help/html/index.html?basic_irc_commands.html

https://dd.ircdocs.horse/refs/commands/nick

https://dd.ircdocs.horse/refs/commands/join

https://youtu.be/6UbKenFipjo?si=MUrXCzxaucvA2Ao9

https://fr.wikipedia.org/wiki/IRC

https://www.ibm.com/docs/fr/i/7.6.0?topic=designs-using-poll-instead-select

Beej's Guide to Network Programming (https://beej.us/guide/bgnet/)
