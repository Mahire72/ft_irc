# ft_irc roadmap

| Phase | Goal                   | Main things you'll learn              |
| ----- | ---------------------- | ------------------------------------- |
| 0     | Understand the project | IRC, client/server, requirements      |
| 1     | C++ project setup      | C++98, Makefile, classes              |
| 2     | TCP basics             | socket, bind, listen, accept          |
| 3     | Non-blocking I/O       | `fcntl`, blocking vs non-blocking     |
| 4     | `poll()`               | event loop, multiple clients          |
| 5     | Client management      | Client class, connections, state      |
| 6     | IRC parsing            | raw messages → commands/arguments     |
| 7     | Authentication         | `PASS`, registration                  |
| 8     | User registration      | `NICK`, `USER`                        |
| 9     | Channels               | JOIN/PART, channel membership         |
| 10    | Messaging              | `PRIVMSG`, broadcasting               |
| 11    | Operators              | permissions, operator users           |
| 12    | Operator commands      | KICK, INVITE, TOPIC                   |
| 13    | MODE                   | `i`, `t`, `k`, `o`, `l`               |
| 14    | Robustness             | disconnects, partial messages, errors |
| 15    | Testing                | multiple clients + edge cases         |
| 16    | Evaluation preparation | subject checklist + debugging         |

