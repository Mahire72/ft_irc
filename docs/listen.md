# Listen()
                socket()
                ↓
                Created a TCP socket
                ↓
                server_fd = 3
                ↓
                bind()
                ↓
                Assigned address + port
                ↓
                Bind successful!
    But we are still not accepting clients.
## 🚦 Next: listen()
    Imagine server has a phone:
                socket()
                ↓
                📞 Phone created  
    We've given it a phone number:
                bind()
                 ↓
                📞 Phone number = port 6667
    But the phone isn't saying: "i'm ready to receive calls". But listen will do this job, she will say:"i'm ready to receive incoming connection requests."

    But listen() does not accept the call. it only puts the socket into listening mode.
                socket()
                ↓
                bind()
                ↓
                listen()
                ↓
                "I'm waiting for clients!"
## syntax of listen 
                int listen(int sockfd, int backlog);
    listen() marks the socket referred to by sockfd as a passive socket, that is, as a socket that will be used to accept incoming connection requests using accept(2).
    source:
    https://man7.org/linux/man-pages/man2/listen.2.html

### backlog
    The `backlog` argument of `listen()` defines the maximum length of the queue of pending connection requests.

### one important distinction
    After bind()
                Server
                │
                └── socket exists
                    │
                    └── assigned port 6667
    After listen()
                Server
                │
                └── socket exists
                    │
                    ├── port 6667
                    │
                    └── waiting for connections
    so the next stop should be accept()
                socket()
                ↓
                bind()
                ↓
                listen()
                ↓
                accept()  ← "A client is here!"
                ↓
                client_fd

                | Function   | Simple meaning                |
                | ---------- | ----------------------------- |
                | `socket()` | 📞 Give me a telephone        |
                | `bind()`   | 📱 Give my telephone a number |
                | `listen()` | 📞 I'm ready for calls        |
                | `accept()` | 🤝 Accept this caller         |

