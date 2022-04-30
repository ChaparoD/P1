#include "./os_API.h"
#include <stdlib.h>
#include <stdio.h>

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

Page* chargeBitMap(FILE* ptr){
    Page* newPage = pageInit();
    seekPage(0, 0, ptr, newPage ->Shells);
    return newPage;
};

void os_close_disk(){
  fclose(ptr);
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