 ###
Client is a program used by the person. 
                Alice
                │
                ▼
                IRC Client
Alice uses the client to connect to your server.
and the client migth say : 
                I want to connect to:

                IP:   192.168.1.100
                Port: 6667

### Server is our ircserv

### TCP
TCP is a way for the client and server to communicate.

                CLIENT                         SERVER
                │                               │
                │──────────── TCP ──────────────│
                │                               │
                │  "Hello"                      │
                │──────────────────────────────►│
                │                               │
                │              "Hi!"            │
                │◄──────────────────────────────│

for FT_IRC we dont need to implement TCP, the opearting system already provides TCP. Our job is to use the socket API to communicate through it.

### SOCKET
Socket is a communication endpoint that your program gets from the operating system.
    socket(....)
        the OS creates a socket for you.
    int fd == 3;

                Your program
                    │
                    │ socket()
                    ▼
                Operating System
                    │
                    ▼
                Socket created
                    │
                    ▼
                fd = 3
later we usde that fd to work with the socket.

### How does server start?
                socket()
                ↓
                bind()
                ↓
                listen()
                ↓
                accept()

## socket()
# 1. OS gives me a socket.
    int server_fd = socket(....);

Conceptually
                ircserv
                │
                │ socket()
                ▼
                Operating System
                │
                ▼
                Socket
## bind()
Bind() will use this IP Adress and taht port/
ex: 
                IP:   192.168.1.100
                Port: 6667
so :
                socket
                │
                ▼
                bind()
                │
                ▼
                192.168.1.100:6667

Now the OS knows that this socket belongs to the service listenning on this address/port

## listen()
listen(....). => Start waiting for incoming connections.
now our server  is now standing at the door:
                SERVER
                   │
                   │
              port 6667
                   │
                   ▼
              🚪 WAITING

## A client arrives
now Alice starts her IRC client. and she wants 
        192.168.1.100:6667
so the connection will be :
Alice's computer                 Your computer

            ┌──────────────┐                 ┌──────────────┐
            │ IRC Client   │                 │   ircserv    │
            └──────┬───────┘                 └──────┬───────┘
                │                                │
                │────────── TCP connection ─────│
                │                                │
                └───────────────────────────────►

our server is waiting with accept();
