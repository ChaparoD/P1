#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "./os_API.h"

#define MAX_SPLIT 255
#define BUFFER_SIZE 4096

static char **split_by_sep(char *str, char *sep)
{
  char **new_str = calloc(MAX_SPLIT, sizeof(char *));
  int index = 0, len;

  char *token = strtok(str, sep);
  while (token != NULL)
  {
    new_str[index] = calloc(BUFFER_SIZE, sizeof(char));
    strcpy(new_str[index++], token);
    token = strtok(NULL, sep);
  }

  // Remove dangling Windows (\r) and Unix (\n) newlines
  len = strlen(new_str[index - 1]);
  if (len > 1 && new_str[index - 1][len - 2] == '\r')
    new_str[index - 1][len - 2] = '\0';
  else if (len && new_str[index - 1][len - 1] == '\n')
    new_str[index - 1][len - 1] = '\0';
  return new_str;
}

/*
 * Reads a line fo user input and returns it as an array of strings
 */
char **read_user_input()
{
  char *input = calloc(BUFFER_SIZE, sizeof(char));
  fgets(input, BUFFER_SIZE, stdin);
  char **split_input = split_by_sep(input, " ");
  free(input);
  return split_input;
}

/*
 * Frees user input obtained by the read_user_input function
 */
void free_user_input(char **input)
{
  for (int i = 0; i < MAX_SPLIT; i++)
  {
    free(input[i]);
  }
  free(input);
}

int main(int argc, char const *argv[])
{
  printf("Hello P1!\n");
  //VARIABLE DE LECTURA DE PAGINA (NO MODIFICAR)
  unsigned short buffer[2048]; //unsigned short = 16 bits ( = tamaño de celda (2 Bytes)) Leemos de a páginas

  printf("argv[0] = %s\n argv[1] = %s\n ", argv[0], argv[1] );
  //char* diskName = argv[1];
  //os_mount(diskName, 145363523, &ptr);
  //ptr = fopen(diskName,"rb");
  int iter = 1;
  char** input;
  while (iter){
    printf("INSERT COMMAND:\n");
    
    input = read_user_input();
    // Entra corriendo ./ssdfs os_mount diskName life  (FUNCIONANDO)
    if (strncmp(input[0],"os_mount", 15) == 0){
      printf("Entering OS_MOUNT\n");
      char* diskname = argv[1];
      unsigned int input_life = atoi(input[1]);
      os_mount(diskname, input_life);
      printf("Disk Mounted\n");
    }
    //Testing retorno de páginas. //Con esta funcion podemos copiar en nuevos structs Page y Blocks.
    // seekPage(0, 6, ptr, buffer);
    // for (int shell = 0; shell < 2048; shell++){
 
    //     printf("%u -  ", buffer[shell]); 
    //     printBinary(buffer[shell]);
    // }
    

    else if (strncmp(input[0],"os_bitmap", 15) == 0){
      printf("Entering OS_BITMAP \n");
      //Se asume que ya se montó el disco
      int num = atoi(input[1]);
      os_bitmap(num);

    }

    else if (strncmp(input[0],"os_lifemap", 15) == 0){
      printf("Entering OS_LIFEMAP \n");
      //Se asume que ya se montó el disco
      int lower = atoi(input[1]);
      int upper = atoi(input[2]);
      os_lifemap(lower, upper);
    }

    else if (strncmp(input[0],"os_exists", 15) == 0){
      char* filename = argv[1];
      os_exists(filename);
    }

    else if (strncmp(input[0],"os_mkdir", 15) == 0){
      char* directoryname = argv[1];
      os_mkdir(directoryname);
    }

    else if (strncmp(input[0],"os_exit", 15) == 0){
      iter = 0;  
      os_close_disk();
    }

    free_user_input(input);
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