#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "comunication.h"
#include "conection.h"
#include "string.h"
#include "pthread.h"

// Aca se definen funciones intermedias que usen los threads (jugadores)

char* request_name(int socket){
  int msg_code = server_receive_id(socket);
  char* client_name = server_receive_payload(socket);

  return client_name; 
}

// Funcion utilizada para crear threads (un thread por jugador)
void *client_thread(void *current_player){
  PlayersInfo* player = (PlayersInfo*) current_player;
  int socket = player->socket_c1;
  player->player_name = request_name(socket); // ACA SE DETIENE HASTA RECIBIR EL NOMBRE
  server_send_message(player->socket_c1, 1, "Nombre registrado");
  int msg_code;

  while(1)
  {
    // Se obtiene el paquete del cliente 1
    msg_code = server_receive_id(socket); // ACA SE DETIENE HASTA RECIBIR UN MENSAJE

    if (msg_code == 1) //El cliente me envió un mensaje a mi (servidor)
    {
      char * client_message = server_receive_payload(socket);
      printf("%s (jugador %i) dice: %s\n", player->player_name, player->player_number, client_message);

      // Le enviaremos un mensaje de respuesta
      char * response = "mucho texto";

      // Le enviamos la respuesta
      server_send_message(socket, 1, response);
    }
    printf("------------------\n");
  }
}

int main(int argc, char *argv[]){
  // Se define una IP y un puerto
  char * IP = argv[2]; // SUGERENCIA: 0.0.0.0
  int PORT = atoi(argv[4]); // SUGERENCIA: 8000
  Lobby* lobby = malloc(sizeof(Lobby)); // lobby es una lista ligada con los jugadores
  lobby->listsize = 0;
  
  printf("Lobby iniciado, a la espera de jugadores\n");
  while(1)
  {
    // Se crea el servidor y se obtienen los sockets del cliente.
    PlayersInfo * player = prepare_sockets_and_get_clients(IP, PORT); // EN ESTA LINEA SE DETIENE HASTA QUE LLEGUE OTRO CLIENT

    //agregamos el jugador al lobby
    if(lobby->listsize == 0)
    {
      lobby->head = player;
      lobby->tail = player;
    }
    else 
    {
      lobby->tail->next = player;
      lobby->tail = player;
    }
    lobby->listsize++;
    player->player_number = lobby->listsize;
    printf("Jugador %i entra al lobby\n", player->player_number);
  
    // Le enviamos al primer cliente un mensaje de bienvenida
    char * welcome;
    sprintf(welcome, "Bienvenido Jugador %i!! Ingrese su nombre de jugador:", player->player_number);
    server_send_message(player->socket_c1, 2, welcome);

    // Creamos un thread para el cliente mas reciente
    pthread_t thread_id;
    int thread = pthread_create(&thread_id, NULL, client_thread, (void*) player);
  }

  return 0;
}
