## bind()
    Bind() needs to know:"Which address and which port should this socket use?"
    ex:
                IP address = 127.0.0.1
                Port       = 6667
    So we need a way to package this information together.
                struct sockaddr_in

## sockaddr_in is basically an adress conatiner
    Think of it as a samll box:
                ┌──────────────────────────────┐
                │       sockaddr_in             │
                │                              │
                │  Address family: IPv4        │
                │  IP address:     127.0.0.1   │
                │  Port:           6667        │
                │                              │
                └──────────────────────────────┘
    Then we give this box to bind().
                sockaddr_in
                    │
                    │ contains
                    ▼
                ┌─────────────┐
                │ IPv4        │
                │ 127.0.0.1   │
                │ port 6667   │
                └──────┬──────┘
                    │
                    ▼
                    bind()
                    │
                    ▼
                server_fd
    so socketaddr_in describes the adress where our socket should listen.

## The structure has important fields.
                struct sockaddr_in
                {
                    sin_family ->what type of adress
                    sin_port -> which port?
                    sin_addr -> Which IP address?
                };

### sin_family
                address.sin_family = AF_INET;
    This tells OS: This adress is an IPv4 address. like socket(AF_INET, SOCK_STREAM, 0)
    So both part are consistent:
                socket()
                │
                └── AF_INET → IPv4

                sockaddr_in
                │
                └── AF_INET → IPv4
### sin_port
    sin_port is a port, like 6667. But Ports in network structures use network byte order.
                adress.sin_port = htons(6667);
    Don't panic about htons(). just remember:
                6667
                ↓
                htons() hotons menas: host to network short
                ↓
                network byte order
    now just remember : sin_port = htons(port);

### sin_addr
    sin_addr is the IP adress. localhost(this same computer) is: 127.0.0.1
    But we want tell the server to listen all local interfaces using:
                address.sin_addr.s_addr = INADDR_ANY;
    Conceptually:
                address.sin_addr.s_addr = htonl(INADDR_ANY);
    
## Adress structure
                struct sockaddr_in address;

                address.sin_family = AF_INET;
                address.sin_port = htons(6667);
                address.sin_addr.s_addr = htonl(INADDR_ANY);

### Time to call bind()
                bind(server_fd, ..., ...);
    The complete idea is:
                server_fd
                    │
                    │
                    │        address
                    │           │
                    ▼           ▼
                ┌─────────────────┐
                │      bind()     │
                └────────┬────────┘
                        │
                        ▼
                    0.0.0.0:6667  
    The server socket is now bound to the port.

### bind() need a cast
                bind(server_fd,
                    (struct sockaddr *)&address,
                    sizeof(address));
    bind() expects a generic address type: struct sockaddr *, but we are using IPv4-specific: struct sockaddr_in . so we convert: sockaddr_in to sockaddr.
    
    Cnceptually:
                struct sockaddr_in
                        │
                        │ IPv4-specific
                        ▼
                    cast to
                        │
                        ▼
                struct sockaddr *
                        │
                        ▼
                    bind()

### include <sys/socket.h>
    provides us the socket API, including:
                socket()
                bind()
                listen()
                accept()

### include <netinet/in.h>
    provides the INternet/IPv4 adress structures and constants, especially:
                struct sockaddr_in
                AF_INET
                INADDR_ANY
    

