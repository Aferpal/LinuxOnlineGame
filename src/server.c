#include "../include/server.h"

void init_server(struct server* s){

    //initialize mutexes
    pthread_mutex_init(&(s->create_lobby), NULL);
    pthread_mutex_init(&(s->players_ready), NULL);
    pthread_mutex_init(&(s->mutex), NULL);
    pthread_mutex_init(&(s->lobby_ready), NULL);
    pthread_mutex_init(&(s->free_lobby), NULL);


    //inital state for mutexes
    pthread_mutex_lock(&(s->lobby_ready));
    pthread_mutex_lock(&(s->players_ready));


    //init socket
    s->accepter = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in addr = {AF_INET, toHex(22000), 0};
    if( bind(s->accepter, (const struct sockaddr*)&addr, sizeof(addr)) == -1 ){
        exit(-1);
    }

    if( listen(s->accepter, 10) == -1 ){
        exit(-1);
    }

    //Set ready players to 0
    s->nplayers_ready = 0;
}

void* handlePlayer(void* args){
    //format the args
    struct __arg__player__handler__* format_args = (struct __arg__player__handler__*) args;

    struct player* player = format_args->p;
    struct server* server = format_args->s;

    //first ensure that we can enter the lobby
    pthread_mutex_lock(&(server->free_lobby));

    //enter our mutex region
    pthread_mutex_lock(&(server->mutex));

    //update amount of players ready
    server->nplayers_ready += 1;

    printf("New player enters the lobby! %d/10\n", server->nplayers_ready);

    //if there is room for more players we free the mutex
    if(server->nplayers_ready<10){
        pthread_mutex_unlock(&(server->free_lobby));
    }else{
        pthread_mutex_unlock(&(server->players_ready));
    }

    //generate the message
    char message[] = "players x";
    message[9] = server->nplayers_ready;

    //let free the mutex
    pthread_mutex_unlock(&(server->mutex));


    //we send the message to our player
    sendto(player->sock, message, 11, 0, (const struct sockaddr*)&player->addr, sizeof(player->addr));



    //if lobby is ready (if players can enter the new lobby)
    pthread_mutex_lock(&(server->lobby_ready));
    pthread_mutex_lock(&(server->players_ready));

    //enter the mutex region
    pthread_mutex_lock(&(server->mutex));

    //decrease the number of players
    server->nplayers_ready -= 1;

    printf("Player exits the lobby! %d/10\n", server->nplayers_ready);

    //if we are not the last player we let another one exit
    if(server->nplayers_ready > 0 ){
        pthread_mutex_unlock(&(server->lobby_ready));
        pthread_mutex_unlock(&(server->players_ready));
    }else{
        //otherwise we are the last player so we let new players come in to this stage area and we let the lobby creator to form another one
        pthread_mutex_unlock(&(server->create_lobby));
        pthread_mutex_unlock(&(server->free_lobby));

    }

    //generate the message (may be stracted to extern functions later)
    char lobby_announce_message[] = "ip x.x.x.x port xx";

    lobby_announce_message[3] = (char)getip3(server->lobby.sin_addr.s_addr);
    lobby_announce_message[5] = (char)getip2(server->lobby.sin_addr.s_addr);
    lobby_announce_message[7] = (char)getip1(server->lobby.sin_addr.s_addr);
    lobby_announce_message[9] = (char)getip0(server->lobby.sin_addr.s_addr);

    lobby_announce_message[16] = (char)((server->lobby.sin_port >> 8) & 0x0ff);
    lobby_announce_message[17] = (char)(server->lobby.sin_port & 0x0ff);

    //free the mutex
    pthread_mutex_unlock(&(server->mutex));

    //send the message with the address of the lobby to be joined
    sendto(player->sock, lobby_announce_message, 19, 0, (const struct sockaddr*)&player->addr, sizeof(player->addr));

    //after all we free the allocated memory and resources
    close(player->sock); //we don't need to communicate again with the player, (maybe wait first for a confirmation?)
    free(player);
    free(format_args);


    //finally we end the thread
    pthread_exit(NULL);
}


void* continuousLobbyCreator(void* args){

    struct server* s = (struct server*)args;

    while(1){

        pthread_mutex_lock(&(s->create_lobby));
        pthread_mutex_lock(&(s->mutex));

        //do work
        //we create a lobby ( a new process, ideally on another computer, we ask it to give us a lobby and we deliver it to the players ( its address ))
        //ideally we simply ask for a lobby


        //#####¡¡¡¡NEEDS TO BE MADE!!!###¡¡IT'S JUST PROVISIONAL CODE TO COMPLETE!!!###
        s->lobby.sin_family = AF_INET;
        s->lobby.sin_addr.s_addr = dotToIp(127, 0, 0, 1);
        s->lobby.sin_port = toHex(25000);

        pthread_mutex_unlock(&(s->lobby_ready));
        pthread_mutex_unlock(&(s->mutex));

    }
    pthread_exit(NULL);
}

void open_server(struct server* s){

    //first we dispatch the thread that creates the lobbys once there are enough players
    pthread_create(&(s->game_launcher), NULL, continuousLobbyCreator, (void*)s);

    //detach it, so that it's resources are freed once the main program ends
    pthread_detach((s->game_launcher));


    printf("Server open waiting for players...\n");
    //we continuously accept and listen fow new incoming players
    while (1)
    {

        struct player* p = (struct player*)malloc(sizeof(struct player));

        socklen_t slen;

        p->sock = accept(s->accepter, (struct sockaddr*)&(p->addr), &slen);

        printf("New player incoming!\n");

        //we pass the thread the params it needs with an specific struct
        struct __arg__player__handler__* args_handler = (struct __arg__player__handler__*)malloc(sizeof(struct __arg__player__handler__));
        args_handler->p = p;
        args_handler->s = s;

        pthread_t owner;

        //dispatch the thread that represents the incoming player
        pthread_create(&(owner), NULL, handlePlayer, (void*)args_handler);
        
        //again we detach it so it is not needed to join it in order to free its resources
        pthread_detach(owner);
        
        
    }
    
}