#include "keyboard.h"


KeyboardDriver::KeyboardDriver(InterruptManager* manager): InterruptHandler(0x21, manager),
dataport(0x60),
commandPort(0x64)
{
  while(commandPort.Read()&0x1)
    dataport.Read();
  commandPort.Write(0xAE); // PIC sends interrupt
  commandPort.Write(0x20); //current state get
  uint8_t status = (dataport.Read()| 1) & ~0x10;
  commandPort.Write(0x60);
  dataport.Write(status);
  dataport.Write(0xF4);
}
KeyboardDriver::~KeyboardDriver(){}
void printsf(const char *);
uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp){
  uint8_t key = dataport.Read();

switch (key) {
  case 0xFA: break;
  case 0x45: case 0xC5: break;
  case 0x02: printsf("1"); break;
     case 0x03: printsf("2"); break;
     case 0x04: printsf("3"); break;
     case 0x05: printsf("4"); break;
     case 0x06: printsf("5"); break;
     case 0x07: printsf("6"); break;
     case 0x08: printsf("7"); break;
     case 0x09: printsf("8"); break;
     case 0x0A: printsf("9"); break;
     case 0x0B: printsf("0"); break;

     case 0x10: printsf("q"); break;
     case 0x11: printsf("w"); break;
     case 0x12: printsf("e"); break;
     case 0x13: printsf("r"); break;
     case 0x14: printsf("t"); break;
     case 0x15: printsf("z"); break;
     case 0x16: printsf("u"); break;
     case 0x17: printsf("i"); break;
     case 0x18: printsf("o"); break;
     case 0x19: printsf("p"); break;

     case 0x1E: printsf("a"); break;
     case 0x1F: printsf("s"); break;
     case 0x20: printsf("d"); break;
     case 0x21: printsf("f"); break;
     case 0x22: printsf("g"); break;
     case 0x23: printsf("h"); break;
     case 0x24: printsf("j"); break;
     case 0x25: printsf("k"); break;
     case 0x26: printsf("l"); break;

     case 0x2C: printsf("y"); break;
     case 0x2D: printsf("x"); break;
     case 0x2E: printsf("c"); break;
     case 0x2F: printsf("v"); break;
     case 0x30: printsf("b"); break;
     case 0x31: printsf("n"); break;
     case 0x32: printsf("m"); break;
     case 0x33: printsf(","); break;
     case 0x34: printsf("."); break;
     case 0x35: printsf("-"); break;

     case 0x1C: printsf("\n"); break;
     case 0x39: printsf(" "); break;
  default:
    char foo[] = "KEYBOARD 0x00";
    char hex[] = "0123456789ABCDEF";
    foo[11] = hex[(key >> 4) &0xF];
    foo[12] = hex[key & 0x0F];
    printsf(foo);
    break;
}



return esp;
}
