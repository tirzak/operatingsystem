#include "types.h"
#include "gdt.h"
#include "interrupts.h"
#include "keyboard.h"
#include "mouse.h"
#include "driver.h"

void printsf(const char* str)
{
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;

    static uint8_t x=0,y=0;

    for(int i = 0; str[i] != '\0'; ++i)
    {
        switch(str[i])
        {
            case '\n':
                x = 0;
                y++;
                break;
            default:
                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i];
                x++;
                break;
        }

        if(x >= 80)
        {
            x = 0;
            y++;
        }

        if(y >= 25)
        {
            for(y = 0; y < 25; y++)
                for(x = 0; x < 80; x++)
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
        }
    }
}

void printHex(uint8_t key){

  char foo[] = "0x00";
  char hex[] = "0123456789ABCDEF";
  foo[0] = hex[(key >> 4) &0xF];
  foo[1] = hex[key & 0x0F];
  printsf(foo);
}

class PrintsfKeyboardEventHandlere : public KeyBoardEventHandler{
public:
  void OnKeyDown(char c)
  {
     char foo[]= " ";
     foo[0] = c;

    printsf(foo);
  }

};

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}

extern "C" void kernelMain(void* multiboot_structure, uint32_t magicnumber){

    printsf("Geeseseqweqweqeqw\n");
    GlobalDescriptorTable gdt;
  InterruptManager interrupts(0x20,&gdt);


  printsf("Initializing Hardware, Stage 1\n");

  DriverManager drvManager;
  PrintsfKeyboardEventHandlere kbhandler;
  KeyboardDriver keyboard(&interrupts, &kbhandler);

  drvManager.AddDriver(&keyboard);
  MouseDriver mouse(&interrupts);
    drvManager.AddDriver(&mouse);
      printsf("Initializing Hardware, Stage 2\n");
    drvManager.ActivateAll();
  printsf("Initializing Hardware, Stage 3\n");
  interrupts.Activate();


    while (1);
}
