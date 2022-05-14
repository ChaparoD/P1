#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "./os_API.h"

int main(int argc, char const *argv[])
{
  printf("Hello P1!\n");
  //VARIABLE DE LECTURA DE PAGINA (NO MODIFICAR)
  unsigned short buffer[2048]; //unsigned short = 16 bits ( = tamaño de celda (2 Bytes)) Leemos de a páginas

  printf("argv[0] = %s\n argv[1] = %s\n ", argv[0], argv[1] );
  FILE *ptr = NULL;
  char* diskName = NULL;
  Page* bitMap = NULL;  //usa una página , entonces trabajamos directamente con la página
  //Block* lifeMapBlock1 = NULL;
  //Block* lifeMapBlock2 = NULL;
  //char* diskName = argv[1];
  //os_mount(diskName, 145363523, &ptr);
  //ptr = fopen(diskName,"rb");
  int iter = 1;
  char** input;
  while (iter){
    printf("Please enter command: \n");

    input = read_user_input();
    // Entra corriendo ./ssdfs os_mount diskName life  (FUNCIONANDO)
    if (strncmp(input[0],"os_mount", 15) == 0){
      printf("Entering OS_MOUNT \n");
      diskName = argv[1];  //Arroja warning pero funciona
      unsigned int life = atoi(input[1]);
      os_mount(diskName, life, &ptr);
      bitMap = chargeBitMap(ptr);
      //lifeMapBlock1 = chargeLifeMap(0);
      //lifeMapBlock2 = chargeLifeMap(1);
      printBinary(bitMap -> Shells[45]);  //testeado
    }
    //Testing retorno de páginas. //Con esta funcion podemos copiar en nuevos structs Page y Blocks.
    // seekPage(0, 6, ptr, buffer);
    // for (int shell = 0; shell < 2048; shell++){
 
    //     printf("%u -  ", buffer[shell]); 
    //     printBinary(buffer[shell]);
    // }
    

    else if (strncmp(input[0],"os_bitmap", 15) == 0){
      printf("Entering OS_BITMAP \n");
      bitMap = chargeBitMap(ptr);
      //Se asume que ya se montó el disco
      int num = atoi(input[1]);
      if (num == 0){
        for(int i = 0; i < 2048; i++){
          printf("Bloque %d   =  ", i+1);
          printBinary(bitMap -> Shells[i]);
        }
      }
      else{
        printf("Bloque %d   =  ", num);
        printBinary(bitMap -> Shells[num -1]);
      }

    }

    iter = 0;
  }
   
}

// PARA TESTEAR ACCESO A DISCO  
  //iter bloques 1024 x2  (0 -> 2047)
  // correr como ./ssdfs > datos.txt      
  //Para ver output en archivo externo (no correr todo, cortar antes sino este es muy pesado y no abrirá)

  // for (int block = 0; block < 1; block++){
  //   printf("block %d\n", block);
  //   //iter de paginas del bloque
  //   for(int page = 0; page < 256 ; page++){
  //     printf("page %d\n", page + 1);
  //     //iter de celdas de cada página (2048 celdas (16bit))
  //     for (int shell = 0; shell < 2048; shell++){
  //       fseek(ptr, page*2048 + block*2048*256, SEEK_SET); //seteamos el punto de partida de lectura
  //       fread(buffer,sizeof(buffer),1,ptr); 
  //       printf("%u -  ", buffer[shell]); 
  //       printBinary(buffer[shell]);
  //     }
  //   }
  // }