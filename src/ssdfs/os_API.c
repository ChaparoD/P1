#include "./os_API.h"
#include <stdlib.h>
#include <stdio.h>

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
void seekPage(int Block, int Page, FILE* disk , unsigned short *buffer){
    fseek(disk, Page*2048 + Block*2048*256, SEEK_SET); //seteamos el punto de partida de lectura
    fread(buffer,sizeof(buffer),1,disk);  // cargamos en buffer
}

void seekBlockDirectory(int entrada, FILE* disk, fileEntrada *buffer){
    fseek(disk, 32 * entrada + 3*2048*256, SEEK_SET);
    fread(buffer->valid, sizeof(uint8_t), 1, disk);
    fread(buffer->pointer, sizeof(uint32_t), 4, disk);
    fread(buffer->name, sizeof(unsigned char), 27, disk);

};

Page* chargeBitMap(FILE* ptr){
    Page* newPage = pageInit();
    seekPage(0, 0, ptr, newPage ->Shells);
    return newPage;
};

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
    printf("%s AAAAAAAA\n", diskName);
    seekPage(0, 0, ptr, bitMap ->Shells);
    printf("AAAAAAAA\n");
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
    Block* lifeMapBlock1 = blockInit();
    Page* page;
    for(current_page = current_page; current_page <= last_page; current_page++){
        page = &(lifeMapBlock1 -> Pages[current_page]);
        seekPage(1, current_page, ptr, page ->Shells);
        for(int i = 0; i < 2048; i++){
          printf("Bloque 1, Pagina %d, Celda %d   =  ", current_page+1, i+1);
          printBinary(page -> Shells[i]);
        }
        free(page->Shells);
    }

    free(lifeMapBlock1->Pages);
    free(lifeMapBlock1);

    current_page = lower;
    last_page = upper;
    if (lower == -1 && upper == -1){
      current_page = 0;
      last_page = 255;
    }
    Block* lifeMapBlock2 = blockInit();
    printf("BLOQUE 2\n");
    for(current_page = current_page; current_page <= last_page; current_page++){
        page = &(lifeMapBlock2 -> Pages[current_page]);
        seekPage(2, current_page, ptr, page ->Shells);
        for(int i = 0; i < 2048; i++){
          printf("Bloque 2, Pagina %d, Celda %d   =  ", current_page+1, i+1);
          printBinary(page -> Shells[i]);
        }
        free(page->Shells);
    }

    free(lifeMapBlock2->Pages);
    free(lifeMapBlock2);

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
      printf("%d", ( (uint8_t) c.valid >> i ) & 1 ? 1 : 0);
    }

    // 4 bytes 
    for (int i = 31; i >= 0; i--)
    {
      printf("%d", ((uint32_t) c.pointer >> i) & 1 ? 1 : 0);
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
  fileEntrada* entrada = fileEntradaInit(1);
  printf("BLOQUE DIRECTORIO\n");
  for (int i = 0; i < 32768; i++)
  {
    printf("entrada %i:\n", i);
    seekBlockDirectory(i, diskName, entrada);
    printBinary_32Bytes(*entrada);
  }
}