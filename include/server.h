#ifndef SERVER_H
#define SERVER_H

#include<netinet/in.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<stdio.h>
#include "player.h"
#include "utils.h"



typedef struct server{
    int accepter; //the socket that accepts new users and stablishes the tcp connection 
    
    int nplayers_ready;

    pthread_t game_launcher; //thread responsible of creating the game lobby



    //mutex
    pthread_mutex_t create_lobby; //mutex for enough players to dispatch game_launcher

    pthread_mutex_t lobby_ready; //mutex for players to enter the game

    pthread_mutex_t free_lobby; //mutex for players to enter the lobby

    pthread_mutex_t mutex; //regular mutex

    pthread_mutex_t players_ready; 

    struct sockaddr_in lobby;

}server;


//struct for thread args
struct __arg__player__handler__{
    struct server * s;
    struct player * p;
};


//initializes the server, it's variables, mutexes and sockets
void init_server(struct server* s);

//starts listening for clients
void open_server(struct server* s);

#endif