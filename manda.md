# ft_irc 自己写一个聊天服务器（Chat Server)
因此我们的项目叫： ircserv. 它就是整个系统中间的服务器。

IRC = Internet Relay Chat. 聊天协议。 客户端和服务器之间通过TCP 通信。
客户端不会随便告诉服务器： "我要聊天“， 而是按照IRC 协议发送规定好的命令。
    例如： NICK alice==> 意思是“我的昵称叫Alice.

    JOIN #42 ===> 我要加入#42 这个频道。

    PRIVMSG #42 : hello! ==>给#42 频道发送“hello!"

所以 你的服务器必须能够：
    接收数据
    ↓
    理解 IRC 命令
    ↓
    判断是否合法
    ↓
    执行操作
    ↓
    给客户端回复

## 🏠 Channel 是什么？
Channel 理解成一个聊天室/房间。

                ircserv
                   │
          ┌────────┴────────┐
          │                 │
        #42              #coding
          │                 │
      ┌───┼───┐         ┌───┼───┐
      │   │   │         │   │   │
    Alice Bob Tom      John Sara Mike

Alice 想加入#42： JOIN #42
所以你的服务器就要判断：
        这个频道存在吗？
            ↓
        允许 Alice 加入吗？
            ↓
        频道满了吗？
            ↓
        需要密码吗？
            ↓
        是不是 invite-only？
            ↓
        可以加入！

### 💬 如果 Alice 发消息呢？
Alice: PRIVMSG #42 :Hello!
服务器收到之后知道：
        发送者：
        Alice

        目标：
        #42

        内容：
        Hello!

然后服务器查看：
        #42
        ├── Alice
        ├── Bob
        └── Tom
然后把消息转发给 Bob 和 Tom：
              ┌──► Bob
              │
Alice ──► Server
              │
              └──► Tom
这就是Relay(转发)。

## 🔌 那 socket 是什么？（计算机网络部分）
客户端和服务器之间需要建立网络连接：
        Client
        │
        │ TCP connection
        ▼
        Server

Socket 可以简单理解成：
        程序用来进行网络通信的一个接口/端点。
服务器会做：
        socket()
        ↓
        bind()
        ↓
        listen()
        ↓
        accept()

大概意思是：
        socket()
        ↓
        创建网络通信端点

        bind()
        ↓
        绑定 IP + port

        listen()
        ↓
        开始等待客户端

        accept()
        ↓
        有客户端来了， 接受它

## 🚦 为什么需要 port？
#### ./ircserv 6667 password
    这里6667 是port 端口。
例如：
        IP address
            │
            ▼
        192.168.1.100
            │
            ├── port 22   → SSH
            ├── port 80   → HTTP
            ├── port 443  → HTTPS
            └── port 6667 → 你的 IRC server
所以：
    ./ircserv 6667 password ==》 启动我的 IRC 服务器，让它在 6667 这个端口等待客户端连接。

### 最后理解 poll() 
假设只有一个客户端： 
            Alice
            │
            ▼
            Server 很简单
但是ft_irc 要求：
        Alice ──┐
        Bob ────┤
        Tom ────┤
        Sara ───┼──► Server
        John ───┤
        Mike ───┘
服务器不能只盯着Alice. 他需要同时关注很多连接。
所以：
                  poll()
                    │
        ┌───────────┼───────────┐
        ▼           ▼           ▼
      Alice         Bob         Tom
      有数据       没数据       有数据
        │                         │
        ▼                         ▼
      处理                        处理
把poll() 暂时理解成： 帮服务器同时观察很多客户端，告诉服务器现在谁需要处理。

###### 🧠 最终的 ft_irc 思维模型
                    IRC CLIENT
                        │
                        │ TCP
                        ▼
                ┌───────────────┐
                │    ircserv    │
                │               │
                │    SERVER     │
                │               │
                │    poll()     │
                └───────┬───────┘
                        │
             ┌──────────┼──────────┐
             ▼          ▼          ▼
           Alice       Bob        Tom
             │          │          │
             └──────────┼──────────┘
                        ▼
                    CHANNELS
                        │
                 ┌──────┴──────┐
                 ▼             ▼
                #42         #coding

然后每一次客户端发送东西：
                        Client
                        │
                        │ "JOIN #42"
                        ▼
                        Server
                        │
                        ├── parse command
                        │
                        ├── check permission
                        │
                        ├── update state
                        │
                        └── send response