#pragma once
#include <stdio.h>
#include <stdlib.h>



struct page;
typedef struct page Page;
struct page {
    // Cada pagina tiene 2048 celdas
    unsigned short* Shells; //arreglo de celdas de 8bits (1 byte)

};

struct block;
typedef struct block Block;
struct block {
    // Cada bloque tiene 256 paginas
    Page* Pages;

};

struct plane;
typedef struct plane Plane;
struct plane {
    // Cada plano tiene 1024 bloques
    //no creo que utilicemos esto ; mejor solo crear bloques que fueron accedidos o modificados para luego 
    //reescribir
    Block* Blocks;
};

struct disc;
typedef struct disc osFile;
struct osFile {

};

//Init Functions
Page* pageInit();
Block* blockInit();
Plane* planeInit();



//Print interpreter 16 bits
void printBinary(unsigned short c);
//Retorna página (4096Bytes) de lectura en arreglo buffer
void seekPage(int Block, int Page, FILE* disk , unsigned short *buffer); //bloques del 0->2047 paginas 0 -> 255
Page* chargeBitMap(FILE* ptr);
//Funciones entregables
void os_mount(char* diskname, unsigned int life, FILE** ptr);
void os_bitmap(unsigned int num);
