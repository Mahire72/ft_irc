#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <poll.h>

void sendData(int sock, const std::string& message) 
{
    send(sock, message.c_str(), message.length(), 0);
    std::cout << "-> " << message;
}

int createBotSocket()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0); // Je cree la socket qui permet le transport d'info
    if (sock == -1) // si ça rate
    {
        std::cerr << "bot socket failed\n";
        return -1;
    }
    return sock; // je renvoie la socket qui est comme un fd
}

bool setBotAddress(struct sockaddr_in *serv_addr, int sock, int port, std::string const &ip)
{
    serv_addr->sin_family = AF_INET; // Définit le protocole a use donc IPv4 
    serv_addr->sin_port = htons(port); // Assigne et convertit le int port en value exigé par les normes réseaux
    serv_addr->sin_addr.s_addr = inet_addr(ip.c_str()); // Convertit l'ip en forme binaire compris par la machine
    if (serv_addr->sin_addr.s_addr == INADDR_NONE) // si jamais la conversion échoue
    {
        std::cerr << "Error: IP invalid.\n";
        close(sock);
        return false;
    }
    return true;
}

int main(int argc, char **argv) 

{
    if (argc != 4) 
    {
        std::cerr << "Usage: ./bot <IP> <Port> <Password>\n";
        return -1;
    }

    std::string ip = argv[1];
    int port = std::atoi(argv[2]);
    std::string password = argv[3];

    int sock = createBotSocket();
    if (sock == -1)
        return -1;

    struct sockaddr_in serv_addr;
    if (setBotAddress(&serv_addr, sock, port, ip) == false)
        return -1;

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) // tente de se connecter au serveur
    {
        std::cerr << "Error: Connexion to server failed.\n";
        close(sock);
        return -1;
    }

    std::cout << "Connected to server " << ip << ":" << port << "\n";

    sendData(sock, "PASS " + password + "\r\n");
    sendData(sock, "NICK ft_bot\r\n");
    sendData(sock, "USER bot 0 * :Bot IRC 42\r\n");

     struct pollfd pfd;
    pfd.fd = sock;
    pfd.events = POLLIN;

    char buffer[1024];

    std::string user_input[3] = {"!ping", "!hello", "!bye"};
    std::string bot_output[3] = {"Pong", "Welcome", "See u"};

    while (true) 
    {
        int poll_count = poll(&pfd, 1, -1);
        
        if (poll_count == -1) 
        {
            if (errno == EINTR)
                continue;
            std::cerr << "Error: poll() failed.\n";
            break;
        }

        if (pfd.revents & POLLIN) // Confirme que la socket a recu des infos à lire 
        {
            std::memset(buffer, 0, sizeof(buffer));
            
            int bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0); // assigne au buffer le msg
            
            if (bytes_received <= 0) // check si il y a une issue 
            {
                std::cout << "Deconnected from server.\n";
                break;
            }

            std::string data(buffer);
            std::cout << "<- " << data;

            if (data.find("PING") == 0) 
            {
                std::string pong = "PONG " + data.substr(5);
                sendData(sock, pong);
            }

            size_t privmsg_pos = data.find("PRIVMSG");
            for (size_t i = 0; i < 3; i++)
            {
                if (privmsg_pos != std::string::npos && data.find(user_input[i]) != std::string::npos) 
                {
                    size_t exclamation_pos = data.find('!');
                    if (exclamation_pos != std::string::npos && data[0] == ':') 
                    {
                        std::string sender = data.substr(1, exclamation_pos - 1);
                        std::string response = "PRIVMSG " + sender + " : " + bot_output[i] + "\r\n";
                        sendData(sock, response);
                    }
                }
            }
        }
    }
    close(sock);
    return 0;
}