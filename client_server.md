## Client 怎么连接 Server?
简单的网络系统：
                小明的电脑                         你的服务器电脑

                ┌─────────────┐                  ┌─────────────┐
                │ Chat Client │                  │ Chat Server │
                └──────┬──────┘                  └──────┬──────┘
                    │                                │
                    │────────── 网络 ────────────────│
                    │                                │
因此小明的程序通过IP address + Port 找到你的服务器。
    IP adress + Port

1. IP Adress =  找到哪台电脑。（192.168.1.100）。 但是找到ip 地址还不够， 因为一台电脑可以同时有很多网络程序。
比如： 
            192.168.1.100

            ├── Web Server
            ├── SSH Server
            ├── Database
            └── IRC Server、
所以通过port 找想找的程序。

2. Port = 找到哪个程序 
            IP:   192.168.1.100
            Port: 6667
可以想象成：
            一栋大楼
        192.168.1.100
                │
        ┌───────┼────────┐
        │       │        │
       门22    门80     门6667
        │       │        │
       SSH    HTTP      IRC
 所以：
    ip adress = 哪台电脑
    port = 哪个网络服务。

### TCP 是什么？
    TCP  是为了Client 和 Server 还需要一种方式来可靠地传输数据。
    ** tcp 是一条client和server之间的可靠通信通道。

        Client                         Server
        │                               │
        │────────── TCP ────────────────│
        │                               │
        │  "Hello"                      │
        │──────────────────────────────►│
        │                               │
        │              "OK"             │
        │◄──────────────────────────────│

tcp 会负责很多底层事情， 比如可靠传输， 数据顺序等。

 ### ft_irc : 使用tcp, 让irc client 和你的 irc server 通信。

 ## Socket 是什么？
程序不能直接说：“tcp! 给我链接”。 程序需要通过操作系统提供的 socket API 来进行网络通信。
    所以：socket() ==》 操作系统， 请给我的程序创建一个网络通信端点。
        你的程序
        │
        │ socket()
        ▼
        操作系统
        │
        ▼
        创建 socket
然后操作系统给你一个数字：
        int server_fd = socket(...);
    例如： server_fd = 3， 这里3 就是一个file descriptor(文件描述符)。

### server 是怎么启动的？
    。/ircserv 6667 password
服务器启动后大概做：
            socket()
            ↓
            创建网络 socket
            ↓
            bind()
            ↓
            绑定 IP + Port
            ↓
            listen()
            ↓
            等待客户端
            ↓
            accept()
            ↓
            客户端来了！
可以想象：
                 YOUR SERVER

             socket()
                 │
                 ▼
            ┌─────────┐
            │ Socket  │
            └────┬────┘
                 │
               bind
                 │
                 ▼
          IP + Port
        192.168.x.x:6667
                 │
              listen
                 │
                 ▼
          等待客户端...

### client 来了。。。。。
现在Alice 使用 IRC Client。 她告诉client:
            Server:
                192.168.1.100

            Port:
                6667    
于是：
        Alice's computer
            │
            │
            │ TCP connection
            │
            ▼
        192.168.1.100:6667
            │
            ▼
        Your ircserv

服务器正在： accept(...).
于是：
        Client
        │
        │ TCP connection
        ▼
        Server
        │
        │ accept()
        ▼
        New client socket
#### **  服务器一开始有一个socket: Server Socket.  工作是等待新的客户端。
当 Alice 连接以后：
        Server Socket
            │
            │ accept()
            ▼
        Client Socket
现在Alice 有了自己的连接。
如果Bob 也连接：
                 Server
                   │
             Server Socket
                   │
          ┌────────┼────────┐
          ▼        ▼        ▼
       Alice FD  Bob FD   Tom FD
所以： 这就是为什么我们的服务器需要管理很多file descriptors.

#### poll() 来了。。。。。。。。。。。
if 
        Alice FD = 4
        Bob FD   = 5
        Tom FD   = 6
服务器需要知道， Alice, Bob, Tom 有没有发送数据？
如果一个一个检查：
        检查 Alice
        ↓
        检查 Bob
        ↓
        检查 Tom
这种方式很麻烦， 而且如果莫个连接阻塞， 就会出现问题。
所以你的irc 使用 poll().
    暂时理解成： “操作系统，帮我观察这些 socket；哪个现在可以读/写/有事件，就告诉我。”

                    poll()
                      │
        ┌─────────────┼─────────────┐
        ▼             ▼             ▼
     Alice FD      Bob FD         Tom FD
        │             │             │
      READY          idle          READY
        │                           │
        ▼                           ▼
      recv()                      recv()
## subject 强调只有一个poll(). 因为你的服务器要用一个统一的事件循环管理所有客户端。


#### 最后加上 IRC
        Client
        │
        │ TCP
        ▼
        Server
        │
        │ poll()
        ▼
        发现 Client 有数据
        │
        ▼
        recv()
        │
        ▼
        IRC message

例如Alice 发送：NICK alice\r\n, 服务器会：
        recv()
         ↓
        "NICK alice"
         ↓
        解析
         ↓
        发现 command = NICK
         ↓
        nickname = alice

# 再比如： JOIN #42\r\n
服务器：
        recv()
        ↓
        "JOIN #42"
        ↓
        解析
        ↓
        找到 channel #42
        ↓
        检查权限
        ↓
        加入 Alice

最后： PRIVMSG #42 :Hello!\r\n
服务器：
        recv()
        ↓
        解析
        ↓
        PRIVMSG
        ↓
        目标 = #42
        ↓
        找到 #42 的用户
        ↓
        send() 给其他用户

#### 🧩 现在把整个 ft_irc 串起来

                    IRC CLIENT
                        │
                        │
                 IP + Port
                        │
                        ▼
                     TCP
                        │
                        ▼
              ┌─────────────────┐
              │     ircserv     │
              │                 │
              │  Server Socket  │
              │       │         │
              │     accept()    │
              │       │         │
              │       ▼         │
              │  Client Socket  │
              │       │         │
              │     poll()      │
              │       │         │
              │       ▼         │
              │     recv()      │
              │       │         │
              │       ▼         │
              │  Parse IRC      │
              │       │         │
              │       ▼         │
              │ Process command  │
              │       │         │
              │       ▼         │
              │    send()        │
              └───────┬─────────┘
                      │
                      ▼
                   CLIENT
ft_irc 就是让你自己写一个 IRC 聊天服务器。客户端通过 TCP 连接你的服务器，服务器使用 socket 接收连接，通过 poll() 同时管理多个客户端，然后接收 IRC 命令、解析命令、修改服务器里的状态（用户/频道/权限），最后把正确的信息发送给客户端。




