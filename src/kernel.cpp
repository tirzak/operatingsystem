#include <common/types.h>
#include <hardwarecommunication/port.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/pci.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <gdt.h>
#include <common/types.h>
using namespace myos;
using namespace myos::common;
using namespace myos::hardwarecommunication;
using namespace myos::drivers;
void printsf(const char *str)
{
  static uint16_t *VideoMemory = (uint16_t *)0xb8000;

  static uint8_t x = 0, y = 0;

  for (int i = 0; str[i] != '\0'; ++i)
  {
    switch (str[i])
    {
    case '\n':
      x = 0;
      y++;
      break;
    default:
      VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | str[i];
      x++;
      break;
    }

    if (x >= 80)
    {
      x = 0;
      y++;
    }

    if (y >= 25)
    {
      for (y = 0; y < 25; y++)
        for (x = 0; x < 80; x++)
          VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ' ';
      x = 0;
      y = 0;
    }
  }
}

void printHex(uint8_t key)
{

  char foo[] = "0x00";
  char hex[] = "0123456789ABCDEF";
  foo[0] = hex[(key >> 4) & 0xF];
  foo[1] = hex[key & 0x0F];
  printsf(foo);
}

class PrintsfKeyboardEventHandler : public KeyBoardEventHandler
{
public:
  void OnKeyDown(char c)
  {
    char foo[] = " ";
    foo[0] = c;

    printsf(foo);
  }
};

class MouseToConsole : public MouseEventHandler
{

  int8_t x, y;

public:
  MouseToConsole()
  {
    static uint16_t *VideoMemory = (uint16_t *)0xb8000;
    x = 40;
    y = 12;
    VideoMemory[80 * y + x] = ((VideoMemory[80 * y + x] & 0xF000) >> 4) | ((VideoMemory[80 * y + x] & 0x0F00) << 4) | ((VideoMemory[80 * y + x] & 0x00FF));
  }
  void OnMouseMove(int xoffset, int yoffset)
  {

    static uint16_t *VideoMemory = (uint16_t *)0xb8000;

    VideoMemory[80 * y + x] = ((VideoMemory[80 * y + x] & 0xF000) >> 4) | ((VideoMemory[80 * y + x] & 0x0F00) << 4) | ((VideoMemory[80 * y + x] & 0x00FF));
    x += xoffset;
    if (x >= 80)
      x = 79;
    if (x < 0)
      x = 0;

    y += yoffset;
    if (y < 0)
      x = 0;
    if (y >= 25)
      x = 24;

    VideoMemory[80 * y + x] = ((VideoMemory[80 * y + x] & 0xF000) >> 4) | ((VideoMemory[80 * y + x] & 0x0F00) << 4) | ((VideoMemory[80 * y + x] & 0x00FF));
  }
};

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
  for (constructor *i = &start_ctors; i != &end_ctors; i++)
    (*i)();
}

extern "C" void kernelMain(void *multiboot_structure, uint32_t magicnumber)
{

  printsf("Welcome to the OS\n");
  GlobalDescriptorTable gdt;
  InterruptManager interrupts(0x20, &gdt);

  printsf("Initializing Hardware, Stage 1\n");

  DriverManager drvManager;
  PrintsfKeyboardEventHandler kbhandler;
  KeyboardDriver keyboard(&interrupts, &kbhandler);
  drvManager.AddDriver(&keyboard);
  MouseToConsole mouseHandler;

  MouseDriver mouse(&interrupts, &mouseHandler);
  drvManager.AddDriver(&mouse);

  PeripheralComponentInterconnectController PCIController;
  PCIController.SelectDrivers(&drvManager);
  printsf("Initializing Hardware, Stage 2\n");
  drvManager.ActivateAll();
  printsf("Initializing Hardware, Stage 3\n");
  interrupts.Activate();

  while (1)
    ;
}
