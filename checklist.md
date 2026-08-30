## Roadmap
    ft_irc
    │
    ├── 1. Understand IRC
    │
    ├── 2. Understand TCP sockets
    │
    ├── 3. Create a basic server
    │
    ├── 4. Accept one client
    │
    ├── 5. Accept multiple clients
    │
    ├── 6. Use poll()
    │
    ├── 7. Parse IRC messages
    │
    ├── 8. Implement commands
    │
    ├── 9. Implement channels
    │
    └── 10. Test + debug + polish

## checklist

# 1. first checklist
    program name:
        ircserv
    Run:
        ./ircserv <port> <password>
    language:
        c++98
    port is used for listening
    Password is required for clients.

# 2. restrictions
    No forking : no fork()
    Non-blocking I/O
        All I/O operations must be non-blocking
        File descriptors must be non-blocking
    
    ** only 1 poll() (or equivalent) can be used for handling all these operations.
        so, Use one poll() structure/mechanism.
        Don't create one poll() per client
        Don't use blocking read(), write(), send(), or recv() outside the polling logic.
Also subject says:
    After a read/recv or write/send, you must not rely on errno to decide that you don't need to send/read again, for example after EAGAIN.

    So add:
        Understand non-blocking read/recv
        Understand non-blocking send/write
        Don't use errno == EAGAIN as your polling strategy

# 3. Networking requirements
Server needs to communicate using: TCP/IP(v4 or v6)

Therefore:
        Create a socket
        Configure socket
        Bind socket to port
        Listen for connections
        Accept clients
        Communicate using TCP
        Support IPv4 or IPv6
        Make sockets non-blocking
        Handle multiple clients

so the basic architecture is:
                    IRC CLIENT
                        │
                        │ TCP
                        ▼
                 ┌─────────────┐
                 │   ircserv   │
                 │             │
                 │   socket    │
                 │     ↓       │
                 │    bind     │
                 │     ↓       │
                 │   listen    │
                 │     ↓       │
                 │   accept    │
                 └──────┬──────┘
                        │
              ┌─────────┼─────────┐
              ▼         ▼         ▼
           Client A  Client B  Client C

# 4. poll() is a major requirement
Subject says: "handle multiple clients simultaneously without hanging." and "only 1 poll() is allowed.

## so poll() cheklist:
        Understand what a file descriptor is
        Understand why sockets have file descriptors
        Understand blocking vs non-blocking
        Understand what poll() does
        Create a pollfd structure/vector
        Add server socket to poll
        Add client sockets to poll
        Detect new connections
        Detect incoming data
        Detect disconnected clients
        Detect writable sockets when necessary
        Handle all clients through the same polling mechanism

Eventually our's server will conceptually look like :
                    poll()
                      │
          ┌───────────┼───────────┐
          │           │           │
          ▼           ▼           ▼
       Server FD   Client A    Client B
          │           │           │
          │           │           │
       new client   message     message
          │           │           │
          ▼           ▼           ▼
       accept()     recv()      recv()

# 5. IRC functionality
The subject says: the reference client must be able to:
        Authenticate
        Set nickname
        Set username
        Join a channel
        Send messages
        Receive messages
So basic cheklist should be:
        CLIENT REGISTRATION
        [ ] PASS
        [ ] NICK
        [ ] USER

        CHANNELS
        [ ] JOIN
        [ ] PART

        MESSAGING
        [ ] PRIVMSG

        CONNECTION
        [ ] QUIT

# 6. Multiple clients
The subject explicitly says messages sent by one client to a channel must be forwarded to the other clients in that channel.
For exm:
        Alice
        │
        │ PRIVMSG #42 :Hello!
        ▼
        ircserv
        │
        ├──────────────► Bob
        │
        ├──────────────► Charlie
        │
        └──────────────► David
So:
        Maintain connected clients
        Maintain channels
        Maintain channel membership
        Find all users in a channel
        Broadcast messages
        Don't send the message back incorrectly to the sender
        Handle clients leaving
This is where our data structures and architecture become important.

# 7. Operators
Subject says:"we need operators and regular users"

so 
    Channel
    ├── Operator
    ├── Operator
    ├── Regular user
    ├── Regular user
    └── Regular user
### checklist
    Know who owns/is operator of a channel
    Know which users are regular users
    Enforce operator permissions
    Prevent unauthorized users from operator commands

# 8. Operator commands
## KICK:Eject a client from the channel
    [KICK]
    Operator
    │
    ▼
    Channel
    │
    └── Remove Bob

Checklist:
    implement KICK
    Check that user issuing it is operator
    Check target exists
    Remove target from channel
    Notify appropriate clients

## INVITE: invite a client to a channel.
### Checklist:
    Implement INVITE
    Check permissions
    Find target client
    Handle invite state if required
    Send appropriate IRC responses

## TOPIC
    change or view the channel topic.
### checklist:
    Implement TOPIC
    View topic
    Change topic
    Check topic permissions
    Notify channel appropriately

## MODE:
requires mode  with i. Invite-only channel: +i.
### checlkist:
    Add invite-only mode
    Remove invite-only mode
    Prevent unauthorized JOINs
t: Topic restriction  +t. mrans only appropriate users/operators can change the topic.
### checklist:
Add +t
Remove -t
Enforce topic restriction

## k Channel password/key
    +k password
checklist:
    Add channel key
    Remove channel key
    Require key when joining
    Don't expose password incorrectly

## o : Operate privilege
    +o username
    -o username
### checklist:
    Give operator privilege
    Remove operator privilege
    Check permissions
## l : user limit : +l 10
### checklist:
    Set channel user limit
    Remove channel user limit
    Reject JOIN when channel is full

