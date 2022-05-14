#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "comunication.h"

typedef struct players_info PlayersInfo;

struct players_info{
  int socket_c1;
  int player_number;
  char* player_name;
  PlayersInfo* next;
};

typedef struct player_list Lobby;

struct player_list{
  PlayersInfo* head;
  PlayersInfo* tail;

  int listsize;
};

PlayersInfo * prepare_sockets_and_get_clients(char * IP, int port);
