## plan
                Server.cpp
                    → Server object + start()

                ServerNetwork.cpp
                    → socket / bind / listen

                ServerPoll.cpp
                    → poll / accept / recv / clients

                           Server
                             │
                    ┌────────┴────────┐
                    │                 │
                ServerNetwork        ServerPoll
                    │                 │
                    │                 │
                socket()             poll()
                setsockopt()          accept()
                fcntl()               recv()
                bind()                clients
                listen()



## connect networking with peraon B's User class.

### first
our server currently has: `_pollFds` which might look like:
                index       fd

                0         3       ← server
                1         4       ← client A
                2         5       ← client B
                3         6       ← client C

This tells us: Which sockets should poll() whatch?. But doesn't tell us NYTHING bout the person using the socket.

Person B's User gives us that information:
                User
                ├── _userFD
                ├── _userIP
                ├── _userBuffer
                ├── _userName
                ├── _userNickname
                └── _isRegistered
so we need a relationship :
                pollfd                         User

                fd = 4  ───────────────────→  User
                                            _userFD = 4
                                            _userNickname = "bob"
                                            _userBuffer = "NICK bob\r\n"

                fd = 5  ───────────────────→  User
                                            _userFD = 5
                                            _userNickname = "alice"
That's our next job.

## add Users to Server
                std::vector<User> _users;

### why std::vector<User>
Because we have many clients. 
_users

[0] → User(fd 4)
[1] → User(fd 5)
[2] → User(fd 6)

`--------------------------`
This is similar to `_pollFds`:
_pollFds

[0] → server fd 3
[1] → client fd 4
[2] → client fd 5
[3] → client fd 6

`The important thing is:`
                pollfd → networking information
                User   → IRC/user information
### when do we create a User?
when `accept()` gives us a new client.
                int clientFd = accept(_severFd, NULL, NULL);
Suppose: `clientFd = 4`;  Then we should cretae: `User user(clientFd);`
                Conceptually:
                client connects
                    ↓
                accept()
                    ↓
                clientFd = 4
                    ↓
                create User(4)
                    ↓
                add User to _users
So the client now has an identify inside your server.

### Modify handleNewConnection()

### But we have a problem
suppose:
_pollFds

index 0 → fd 3 → server
index 1 → fd 4 → Alice
index 2 → fd 5 → Bob

and:
_users

index 0 → fd 4 → Alice
index 1 → fd 5 → Bob

Notice:
poll index 1 -> User index 0
poll index 2 -> User index 1

They're not the same index because the server occupies position 0 in _pollFds.
so we dont do `_users[i]`


IRC command keywords are case-insensitive. The RFC defines command as a sequence of letters and the protocol uses commands such as NICK, PASS, and USER; IRC command handling should not depend on the capitalization the client used