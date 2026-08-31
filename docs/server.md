# server
    My responsibility is to turn te current:
                socket()
                → bind()
                → listen()
                → accept()
                → program ends
    into a real server loop:
                socket()
                → setsockopt()
                → fcntl(O_NONBLOCK)
                → bind()
                → listen()
                → poll()
                ↓
                ├── new client? → accept()
                │                  ↓
                │              add client FD
                │
                └── client data? → later, Person B / shared work
## Step 1 : Make the server socket non-blocking
    Subject requires : All I/O operations must be non-blocking.
    So our listening socket needs to be non-blocking. So we need :
                #include <fcntl.h>
### after socket():
                if (fcntl(server_fd, F_SETFL, O_NONBLOCK) == -1)
                {
                    std::cerr << "fcntl() failed" << std::endl;
                    close(server_fd);
                    return 1;
                }
    So our flows becomes:
                socket()
                ↓
                fcntl(O_NONBLOCK)
                ↓
                bind()
                ↓
                listen()
    Why we need to use this? because previously we have accept(server, ....);
    and could make our program wait:
                accept()
                │
                │
                │ WAITING...
                │
                │ WAITING...
                │
                ▼
                client connects
    so this could make the server socket blocking.

## step2: Add SO_REUSEADDR
### before bind():
                int opt = 1;

                if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
                {
                    std::cerr << "setsockopt() failed" << std::endl;
                    close(server_fd);
                    return 1;
                }
    SO our flow like this:
                socket()
                ↓
                setsockopt(SO_REUSEADDR)
                ↓
                fcntl(O_NONBLOCK)
                ↓
                bind()
                ↓
                listen()

### why SO_REUSEDADDR
### What is SO_REUSEADDR?

`SO_REUSEADDR` is a socket option that allows a server to reuse a
local address/port when the previous connection may still be in
the `TIME_WAIT` state.

Without it, restarting the server quickly may cause:

                bind() failed
                Address already in use

It must be enabled with `setsockopt()` before calling `bind()`.

The typical order is:

                socket()
                    ↓
                setsockopt(SO_REUSEADDR)
                    ↓
                bind()
                    ↓
                listen()
    the code is like this:
                int opt = 1;

                if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
                            &opt, sizeof(opt)) == -1)
                {
                    std::cerr << "setsockopt() failed" << std::endl;
                    close(server_fd);
                    return 1;
                }

                if (bind(server_fd,
                        (struct sockaddr *)&address,
                        sizeof(address)) == -1)
                {
                    std::cerr << "bind() failed" << std::endl;
                    close(server_fd);
                    return 1;
                }
So our flow is like this:
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
                poll()
                ↓
                accept()
        