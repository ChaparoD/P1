#include "./os_API.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 

// os_tree y os_exists

FILE *ptr;
unsigned int life;
char* diskName;

Page* pageInit(){
    Page* newPage = calloc(1, sizeof(Page));
    newPage -> Shells = calloc(2048, sizeof(unsigned short));
    //printf("Tamaño 16 shells: %lu", sizeof(newPage -> Shells[0])); //bit a bit
    return newPage;
}

Block* blockInit(){
    Block* newBlock = calloc(1, sizeof(Block));
    newBlock -> Pages = calloc(256, sizeof(Page*));
    for (int i = 0; i < 256; i++){
        newBlock -> Pages[i] = *pageInit();
    }
    return newBlock;

}

Plane* planeInit(){
    Plane* newPlane = calloc(1, sizeof(Plane));
    newPlane -> Blocks = calloc(256, sizeof(Block*));
    for (int i = 0; i < 1024; i++){
        newPlane -> Blocks[i] = *blockInit();
    }
    return newPlane;
}

// https://stackoverflow.com/questions/7863499/conversion-of-char-to-binary-in-c
//interprete de Binario de 16 bit
void printBinary(unsigned short c){
    for( int i = 15; i >= 0; i-- ) {
        printf( "%d", ( c >> i ) & 1 ? 1 : 0 );
    }
    printf("\n");
}

void printBinaryBytes(unsigned char c){
    for( int i = 7; i >= 0; i-- ) {
        printf( "%d", ( c >> i ) & 1 ? 1 : 0 );
    }
    printf("\n");
}

void seekPage(int Block, int Page, FILE* disk , unsigned short *buffer){
    fseek(disk, Page*2048 + Block*2048*256, SEEK_SET); //seteamos el punto de partida de lectura
    fread(buffer,sizeof(buffer),1,disk);  // cargamos en buffer
}

void seekPageBytes(int Block, int Page, FILE* disk , unsigned char *buffer){
    fseek(disk, Page*2048 + Block*2048*256, SEEK_SET); //seteamos el punto de partida de lectura
    fread(buffer,sizeof(buffer),1,disk);  // cargamos en buffer
}

void seekBlockDirectory(int entrada, FILE* disk, fileEntrada *buffer){
    fseek(disk, 32 * entrada + 3*2048*256, SEEK_SET);
    fread(buffer->valid, sizeof(buffer->valid), 1, disk);
    fread(buffer->pointer, sizeof(buffer->pointer), 4, disk);
    fread(buffer->name, sizeof(buffer->name), 27, disk);

};

Page* chargeBitMap(FILE* ptr){
    Page* newPage = pageInit();
    seekPage(0, 0, ptr, newPage ->Shells);
    return newPage;
};

void os_close_disk(){
  fclose(ptr);
}

// bloque directorio
fileEntrada* fileEntradaInit(int n_entradas)
{
  fileEntrada* file_entrada = calloc(n_entradas, sizeof(fileEntrada));
  
  return file_entrada;
}

directoryBlock* directoryBlockInit()
{
  directoryBlock* new_block = calloc(1, sizeof(directoryBlock));
  new_block->entradas = fileEntradaInit(32768);

  return new_block;
}

//------------------------------------------------------------------//
//  ===============   FUNCIONES  ENTREGABLES ====================   //
//------------------------------------------------------------------//


void os_mount(char* diskname, unsigned int input_life){
    ptr = fopen(diskname,"rb");  // r for read, b for binary
    life = input_life;
    diskName = diskname;
    //"simdiskfilled.bin"
}

void os_bitmap(unsigned int num){
    Page* bitMap = pageInit();
    seekPage(0, 0, ptr, bitMap ->Shells);
    if (num == 0){
      for(int i = 0; i < 2048; i++){
        printf("Celda %d   =  ", i+1);
        printBinary(bitMap -> Shells[i]);
      }
    }
    else{
      printf("Celda %d   =  ", num);
      printBinary(bitMap -> Shells[num -1]);
    }

    free(bitMap -> Shells);
    free(bitMap);
}

void os_lifemap(int lower, int upper){
    int current_page = lower;
    int last_page = upper;
    if (lower == -1 && upper == -1){
      current_page = 0;
      last_page = 255;
    }
    Page* page;
    for(current_page = current_page; current_page <= last_page; current_page++){
        page = pageInit();
        seekPage(1, current_page, ptr, page ->Shells);
        for(int i = 0; i < 2048; i++){
          printf("Bloque 1, Pagina %d, Celda %d   =  ", current_page+1, i+1);
          printBinary(page -> Shells[i]);
        }
        free(page->Shells);
        free(page);
    }

    current_page = lower;
    last_page = upper;
    if (lower == -1 && upper == -1){
      current_page = 0;
      last_page = 255;
    }
    for(current_page = current_page; current_page <= last_page; current_page++){
        page = pageInit();
        seekPage(2, current_page, ptr, page ->Shells);
        for(int i = 0; i < 2048; i++){
          printf("Bloque 2, Pagina %d, Celda %d   =  ", current_page+1, i+1);
          printBinary(page -> Shells[i]);
        }
        free(page->Shells);
        free(page);
    }

}

// void os_tree(){

//     fileDirectory* block_directorio = fileDirectoryInit();
//     seekBlockDirectory(ptr, block_directorio->pointer);
//     for (size_t i = 0; i < count; i++)
//     {
//         /* como recorremos esta wea */;
//     }
// }

void printBinary_32Bytes(fileEntrada c){
    
    // byte valid
    for (int i = 7; i >= 0; i--)
    {
      printf("%d", ( (u_int8_t) c.valid >> i ) & 1 ? 1 : 0);
    }

    // 4 bytes 
    for (int i = 31; i >= 0; i--)
    {
      printf("%d", ((u_int32_t) c.pointer >> i) & 1 ? 1 : 0);
    }

    // 27 bytes

    for (int i = 215; i >= 0; i--)
    {
      printf("%d", ((unsigned char) c.name >> i ) & 1 ? 1 : 0);
    }

    printf("\n");
}

int os_exists(char* filename)
{
  char* extracted_name[27];
  int current_byte = 1;
  for(int pagenum = 0; pagenum < 256; pagenum++){
    Page* newPage = pageInit();
    newPage -> Bytes = calloc(4096, sizeof(unsigned char));
    seekPageBytes(3, pagenum, ptr, newPage->Bytes);
    for(int i = 5; i < 4096; i++){
      char asciibyte = newPage->Bytes[i];
      //printf("Contenido byte %d: %c\n", i+1, newPage->Bytes[i]);
      //printBinaryBytes(newPage->Bytes[i]);
      extracted_name[current_byte-1] = asciibyte;
      if(current_byte == 27){
        i += 5;
        current_byte = 1;
        if (strncmp(filename,extracted_name, 30) == 0){ 
          free(newPage->Bytes);
          free(newPage->Shells);
          free(newPage);
          return 1;
        }
        else{
          strncpy(extracted_name, "", sizeof(extracted_name));
        } 
      }
      else{
        current_byte++;
      }
    }
    free(newPage->Bytes);
    free(newPage->Shells);
    free(newPage);
  }

  printf("File does not exist\n");
  return 0;
}