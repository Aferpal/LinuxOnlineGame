#include<netinet/in.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

#define toHex(_port) ((_port>>8&0x00ff) + (_port<<8&0xff00))
#define getip0(ip) ( ip >> 24 ) & 0x0ff
#define getip1(ip) ( ip >> 16 ) & 0x0ff
#define getip2(ip) ( ip >> 8 ) & 0x0ff
#define getip3(ip) ( ip >> 0 ) & 0x0ff

int main(){
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in myaddress = {AF_INET, 0, 0};
    struct sockaddr_in server = {AF_INET, toHex(22000), 0};
    struct sockaddr_in lobby;

    if(bind(fd, (const struct sockaddr*)&myaddress, sizeof(myaddress))){
        exit(-1);
    }

    if(connect(fd, (const struct sockaddr*)&server, sizeof(server))){
        exit(-1);
    }

    write(STDOUT_FILENO, "\n\tWAITING FOR PLAYERS...\n", 26);
    

    char buffer_reader[256] = {0};

    recv(fd, buffer_reader, 256, 0);

    int nplayers = (int)buffer_reader[9];

    printf("Number of current players: %d/10\n", nplayers);

    recv(fd, buffer_reader, 256, 0);

    printf("Lobby filled, entering a game: \n");

    printf("Lobby = { ip: %d.%d.%d.%d, port: %d }\n", buffer_reader[3], buffer_reader[5], buffer_reader[7], buffer_reader[9], toHex(buffer_reader[17]+buffer_reader[16]*256));

    //to be done, connecting to the lobby and essentially starting the game

    close(fd);
    


    return 0;
}


