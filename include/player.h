#ifndef PLAYER_H
#define PLAYER_H

#include<netinet/in.h>
#include<pthread.h>

typedef struct player{

    int sock;
    struct sockaddr_in addr;
    pthread_t owner;
    
}player;

#endif 