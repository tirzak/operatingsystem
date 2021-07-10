#include "types.h"
#include "gdt.h"


void printsf(char* str){
 static uint16_t* VideoMem = (uint16_t*)0xb8000;
 for (int i = 0; str[i] !='\0'; ++i){
    VideoMem[i] = (VideoMem[i] & 0xFF00) | str[i];  /* code */
 }

}

typedef void (*constructor)();
extern constructor start_ctors;
extern constructor end_ctors;
extern void callConstructors()
{
  for(constructor* i = &start_ctors; i !=&end_ctors; ++i){
    (*i)();
  }

}


extern void kernelMain(void* multiboot_structure, uint32_t magicnumber){

    printsf("Geese");
    gdt_install();
    while (1);
}
