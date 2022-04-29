#include "./os_API.h"
#include <stdlib.h>
#include <stdio.h>




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


//------------------------------------------------------------------//
//  ===============   FUNCIONES  ENTREGABLES ====================   //
//------------------------------------------------------------------//


void os_mount(char* diskname, unsigned int life, FILE** ptr){
    *ptr = fopen(diskname,"rb");  // r for read, b for binary
    //"simdiskfilled.bin"
}