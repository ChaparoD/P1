#include <stdio.h>
#include <stdlib.h>
#include "./os_API.h"

int main(int argc, char const *argv[])
{
  printf("Hello P1!\n");
  Plane* plano1 = planeInit();

unsigned char buffer[10];
FILE *ptr;

ptr = fopen("test.bin","rb");  // r for read, b for binary

fread(buffer,sizeof(buffer),1,ptr); 
for(int i = 0; i<10; i++){
  printf("%u ", buffer[i]); // prints a series of bytes
  }
    
}
