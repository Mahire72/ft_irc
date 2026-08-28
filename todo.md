# FT_IRC CHECKLIST

## 1. Project setup
[ ] C++98
[ ] Makefile
[ ] ircserv executable
[ ] ./ircserv <port> <password>
[ ] Correct compilation flags

## 2. Restrictions
[ ] No fork()
[ ] Non-blocking I/O
[ ] One poll() / equivalent
[ ] No blocking read/write/send/recv
[ ] Correct handling of disconnects/errors

## 3. TCP server
[ ] socket()
[ ] setsockopt()
[ ] bind()
[ ] listen()
[ ] accept()
[ ] close()
[ ] Non-blocking sockets
[ ] IPv4/IPv6

## 4. poll()
[ ] Understand poll()
[ ] Server FD in poll()
[ ] Client FDs in poll()
[ ] Detect new clients
[ ] Detect incoming data
[ ] Detect disconnection
[ ] Handle multiple clients

## 5. Client
[ ] Client class/data
[ ] Socket FD
[ ] Nickname
[ ] Username
[ ] Authentication state
[ ] Connected/disconnected state
[ ] Channels joined

## 6. Registration
[ ] PASS
[ ] NICK
[ ] USER
[ ] Registration validation
[ ] Correct IRC responses

## 7. Channels
[ ] Channel class/data
[ ] Create channel
[ ] Join channel
[ ] Leave channel
[ ] Channel membership
[ ] Channel operators
[ ] Regular users
[ ] Channel topic
[ ] Channel modes

## 8. Basic commands
[ ] JOIN
[ ] PART
[ ] PRIVMSG
[ ] QUIT

## 9. Operator commands
[ ] KICK
[ ] INVITE
[ ] TOPIC
[ ] MODE

## 10. MODE
[ ] +i / -i
[ ] +t / -t
[ ] +k / -k
[ ] +o / -o
[ ] +l / -l

## 11. Messaging
[ ] Client -> server
[ ] Server -> client
[ ] Private message
[ ] Channel message
[ ] Broadcast to channel
[ ] Correct recipients

## 12. Error handling
[ ] Invalid command
[ ] Invalid nickname
[ ] Missing arguments
[ ] Wrong password
[ ] Unknown client
[ ] Unknown channel
[ ] Permission errors
[ ] Disconnected client
[ ] Partial messages
[ ] Multiple messages in one recv()

## 13. Testing
[ ] 1 client
[ ] 2 clients
[ ] Many clients
[ ] Join/part
[ ] Messaging
[ ] Operator commands
[ ] Channel modes
[ ] Disconnects
[ ] Invalid input
[ ] Stress testing

## 14. Final
[ ] Memory leaks
[ ] No crashes
[ ] No hanging
[ ] Clean code
[ ] Makefile clean/fclean/re
[ ] Subject requirements checked
[ ] Evaluation scenarios tested