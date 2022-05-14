#pragma once
#include <stdio.h>
#include <stdlib.h>



struct page;
typedef struct page Page;
struct page {
    // Cada pagina tiene 2048 celdas
    unsigned short* Shells; //arreglo de celdas de 8bits (1 byte)
    enum page_types {pBITMAP, pLIFEMAP, pDIRECTORY, pINDEX, pDATA} page_type;
    int page_num;

    // atributos por si es un bloque LIFEMAP
    u_int32_t pr_ops_quantity;

};

struct block;
typedef struct block Block;
struct block {
    // Cada bloque tiene 256 paginas
    Page* Pages;
    Block* next;
    int block_num;

    enum block_types {bBITMAP, bLIFEMAP, bDIRECTORY, bINDEX, bDATA} block_type;
};

struct plane;
typedef struct plane Plane;
struct plane {
    // Cada plano tiene 1024 bloques
    //no creo que utilicemos esto ; mejor solo crear bloques que fueron accedidos o modificados para luego 
    //reescribir
    Block* Blocks;
};

struct linked_list;
typedef struct linked_list LinkedList;
struct linked_list {
    Block* first;
    Block* tail;
};

struct disc;
typedef struct disc osFile;
struct disc {

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
Block* chargeLifeMap(int block_num);
//Funciones entregables
void os_mount(char* diskname, unsigned int life, FILE** ptr);
void os_bitmap(unsigned num);
void os_lifemap(int lower, int upper);
