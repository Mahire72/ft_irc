# fcntl(O_NONBLOCK)

## step1 fcntl() + O_NONBLOCK
    Our current flow is:
                socket()
                ↓
                setsockopt()
                ↓
                bind()
                ↓
                listen()
                ↓
                accept()
    Our plan is add fcntl(O_NONBLOCK)
                socket()
                ↓
                setsockopt()
                ↓
                fcntl(O_NONBLOCK)
                ↓
                bind()
                ↓
                listen()
                ↓
                accept()
## What problem are we solving?
    BY default, many socket operations can block.
                accept(server_fd, NULL, NULL);
    If nobody is connecting, the program can sit there waiting:
                accept()
                │
                │
                │ waiting...
                │
                │ waiting...
                │
                ▼
                client connects
    `For an IRC server, we don't want our whole program stuck waiting for one operation. But we want:"Try the operation. If nothing is ready, don't freeze my server; let me continue."`

    That's what non-blocking mode helps us achieve.

## What is O_NONBLOCK?
    O_NONBLOCK is a flag that tells the operating system: 
    "Dont't make this file edescriptor wait/block when the operation can't be completed immediately."

                fcntl(server_fd, F_SETFL, O_NONBLOCK);
                we can describ as like this:

                fcntl()
                ↓
                change settings of server_fd
                ↓
                F_SETFL
                ↓
                set the file status flags
                ↓
                O_NONBLOCK
                ↓
                make it non-blocking 

### O_NONBLOCK

By default, socket operations can block while waiting for an event.

`O_NONBLOCK` configures a file descriptor so that operations do not wait indefinitely when they cannot be completed immediately.

We use `fcntl()` to set the `O_NONBLOCK` flag:

    fcntl(fd, F_SETFL, O_NONBLOCK);

This is important for ft_irc because the server must handle multiple
clients without blocking on a single socket.

## What is fcntl()
    It means file control. and it can modify properties of a file descriptor.

### ex:
    int server_fd = socket(...); maight give server_fd = 3;
    so fcntl(server_fd,...) ==> means chnage sth about file description 3. for us that "sth"means o_nonblock

### where to add?
                server_fd = socket(AF_INET, SOCK_STREAM, 0);

                if (server_fd == -1)
                {
                    std::cerr << "socket() failed" << std::endl;
                    return 1;
                }

                if (fcntl(server_fd, F_SETFL, O_NONBLOCK) == -1)
                {
                    std::cerr << "fcntl() failed" << std::endl;
                    close(server_fd);
                    return 1;
                }

## F_SETFL
    Means set the file sataus flags.
                fcntl(server_fd, F_SETFL, O_NONBLOCK);
    Means: Set the file status flags of server_fd so that it is non-blocking.
    The three arguments are:
                fcntl(
                    server_fd,     ← which FD?
                    F_SETFL,       ← what do I want to do?
                    O_NONBLOCK     ← which flag?
                );
## The telephone analogy
#### without non-blocking
                accept()
                ↓
                📞 "Wait until somebody calls."
                ↓
                😴 WAIT...
                ↓
                😴 WAIT...
                ↓
                Client calls
                ↓
                continue
                
#### with non-blocking:
                accept()
                ↓
                📞 "Is somebody calling?"
                ↓
                No?
                ↓
                "Okay, I'll come back later."

#####    "If we're using poll(), why do we need non-blocking sockets?"
    Because poll() tells us that an FD is ready for an operation, but robust server code still shouldn't assume every subsequent operation will always succeed immediately.

    For ft_irc, your subject explicitly requires non-blocking I/O, so we configure the sockets accordingly.

                    poll()
                      │
                      ▼
                Is something ready?
                 /          \
               No            Yes
               │              │
               │              ▼
               │           accept()
               │              │
               │              ▼
               │          new client
               │
               └──────────► poll() again

## IMPORTANT CLIENT_FD
    accept() give us client_fd. So we also need to make the client socket non_bloking.
                fcntl(client_fd, F_SETFL, O_NONBLOCK);

                server_fd
                │
                └── O_NONBLOCK

                client_fd
                │
                └── O_NONBLOCK

### O_NONBLOCK

By default, socket operations can block while waiting for an event.

`O_NONBLOCK` configures a file descriptor so that operations do not
wait indefinitely when they cannot be completed immediately.

We use `fcntl()` to set the `O_NONBLOCK` flag:

    fcntl(fd, F_SETFL, O_NONBLOCK);

This is important for ft_irc because the server must handle multiple
clients without blocking on a single socket.