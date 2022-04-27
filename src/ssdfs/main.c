#include <stdio.h>
#include <stdlib.h>
#include "./os_API.h"

int main(int argc, char const *argv[])
{
  printf("Hello P1!\n");
  //Plane* plano1 = planeInit();

  unsigned long int size = 2147483648;
  unsigned char buffer[size];
  FILE *ptr;

  ptr = fopen("simdiskfilled.bin","rb");  // r for read, b for binary

  fread(buffer,sizeof(buffer),1,ptr); 
  for(int i = 0; i< 100; i++){
    printf("%u -  ", buffer[i]);
    printBinary(buffer[i]);
  }
    
}
