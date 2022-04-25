#pragma once



struct page;
typedef struct page Page;
struct page {
    // Cada pagina tiene 2048 celdas
    int* Shells; //cada int son 32 bits x lo tanto 16 Shells

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
    Block* Blocks;
};

struct disc;
typedef struct disc osFile;
struct osFile {
    Plane* planeOne;
    Plane* planeTwo;
};

//Init Functions
Page* pageInit();
Block* blockInit();
Plane* planeInit();

