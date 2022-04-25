#include "./os_API.h"
#include <stdlib.h>
#include <stdio.h>




Page* pageInit(){
    Page* newPage = calloc(1, sizeof(Page));
    newPage -> Shells = calloc(128, sizeof(int));
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

