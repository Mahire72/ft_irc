# ft_irc

        | ft_irc concept             | What you're actually learning           | Useful in jobs               |
        | -------------------------- | --------------------------------------- | ---------------------------- |
        | TCP sockets                | How computers communicate over networks | Backend, DevOps, networking  |
        | `poll()`                   | Handling many connections efficiently   | Backend, distributed systems |
        | Client/server architecture | How services communicate                | Backend, cloud               |
        | IRC protocol               | Implementing a real specification       | API/protocol development     |
        | Parsing commands           | Turning raw input into structured data  | Backend, compilers, APIs     |
        | Multiple clients           | Concurrency and state management        | Backend, distributed systems |
        | Error handling             | Designing robust software               | **Every software job**       |
        | C++ classes                | Object-oriented design                  | C++/backend jobs             |
        | Debugging                  | Finding problems in complex systems     | **Every developer job**      |
        | Git/team work              | Working like a professional developer   | **Every developer job**      |

## 🧠 The biggest lesson: abstraction
    sendMessage(user, "hello"); 
so some library/frameworl handles everything underneath.

## with ft_irc, we are forced to understand closer to:
        Application
            ↓
        IRC protocol
            ↓
        Your server
            ↓
        poll()
            ↓
        socket
            ↓
        TCP
            ↓
        Operating system
            ↓
        Network

so later when we use things like :
        HTTP
        REST APIs
        WebSockets
        databases
        microservices
        Docker
        Kubernetes
        cloud infrastructure
    we will have much better mental model of what is actually happening underneath.

## 💼 How does this help your career?
An interviewer might ask:
    "How would you handle 10,000 connected clients?"
best answer is: 
    We need a server socket, non-blocking I/O, an     event loop, connection state, and some mechanism such as poll/epoll to monitor many sockets

## 🔥Skills you'll gain
### 1. Networking
    Client->TCP connection->Server->messages->response
    we will learn concepts like this
    IP adresses, ports, TCP connections. sockets, bind(), listen(), accept(), send(), recv() and connection lifecycle.
This is verfy udeful for backend, networking, DevOps, security, distributed systems, etc.

### 2. Concurrency/event-driven programming
imagine we have:
        Client A ─┐
        Client B ─┤
        Client C ─┼──> Your server
        Client D ─┤
        Client E ─┘
Our server can't simply do:
        handle A
        wait
        handle B
        wait
        handle C
so we need to efficently manage many connections. so we use poll().
also understanding this will make concepts such as: event loops, asynchronous programming, WebSockets, epoll, kqueue and asynchronous servers musch easier later.

### 3. software architecture
we will learn start separating responsibilities:
Server
 ├── Client
 ├── Channel
 ├── Command
 │    ├── JOIN
 │    ├── PRIVMSG
 │    ├── MODE
 │    └── ...
 └── Parser
 this will teach use :"How to structure a real program."

 ### 4. protocol implementation
    IRC is a protocol, we are not inventing arbitrary behavior, We're implementing rules defined by a specification.
This is very similar to professional work involving:
        HTTP
        SMTP
        FTP
        WebSocket
        MQTT
        custom APIs
We will learn to ask:"What does the specification say?"

### 5. Debugging
        Client connects
            ↓
        JOIN works
            ↓
        PRIVMSG works
            ↓
        Client disconnects
            ↓
        💥 server crashes
So we have to figure out why.
also we learn tools and techniques such as:
        gdb
        logs
        assertions
        sanitizers
        memory debugging
        network inspection
        systematic reproduction of bugs
So being good at debugging is one of the most valuable skills a developer can have.


## understand the base and logical behind every fuction.
ex:
    Without poll:

        Client A
        ↓
        wait...
        ↓
        Client B
        ↓
        wait...
        ↓
        Client C
with poll():
              ┌─ Client A
              ├─ Client B
poll() ───────┼─ Client C
              ├─ Client D
              └─ Client E

## 🌍 Where these skills transfer
After ft_irc, you can connect the knowledge to:

                    ft_irc
                       │
          ┌────────────┼────────────┐
          ↓            ↓            ↓
      Networking     Backend      Systems
          │            │            │
       TCP/IP       REST APIs     Linux
       sockets      WebSockets    processes
       poll         databases     I/O
          │            │            │
          └────────────┼────────────┘
                       ↓
                  Cloud / DevOps
                       ↓
               Distributed Systems
### advice of chatpapa:
    For every major part, ask yourself:
    What problem does this solve?
    Why does the operating system work this way?
    What happens underneath?
    How would this behave with 1,000 clients?
    How would I explain this to an interviewer?
