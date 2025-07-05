#ifndef CLIENT_H_
#define CLIENT_H_

#include<netinet/in.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include"utils.h"
 
typedef struct sockaddr_in address_t;

address_t join_game();

#endif