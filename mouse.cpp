#include "mouse.h"


MouseDriver::MouseDriver(InterruptManager* manager): InterruptHandler(0x2C, manager),
dataport(0x60),
commandPort(0x64)
{
  offset = 0;
  buttons = 0;
  static uint16_t* VideoMemory = (uint16_t*)0xb8000;

  VideoMemory[80*12+40] = ((VideoMemory[80*12+40] & 0xF000)>>4)
  | ((VideoMemory[80*12+40] & 0x0F00)<<4)
  | ((VideoMemory[80*12+40] & 0x00FF));

  commandPort.Write(0xA8); // PIC sends interrupt
  commandPort.Write(0x20); //current state get
  uint8_t status = (dataport.Read()| 2);
  commandPort.Write(0x60);
  dataport.Write(status);
  commandPort.Write(0xD4);
  dataport.Write(0xF4);
  dataport.Read();
}
MouseDriver::~MouseDriver(){}
void printsf(const char *);
uint32_t MouseDriver::HandleInterrupt(uint32_t esp){
  uint8_t status = commandPort.Read();
  if(!(status & 0x20))
    return esp;

 static int8_t x=40,y=12;
  buffer[offset] = dataport.Read();
  offset = (offset + 1)%3;

  if (offset == 0){
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;


    VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0xF000)>>4)
    | ((VideoMemory[80*y+x] & 0x0F00)<<4)
    | ((VideoMemory[80*y+x] & 0x00FF));
    x+=buffer[1];
if(x>=80) x =79;
    if(x<0) x= 0;



    y-= buffer[2];
    if(y<0) x= 0;
    if(y>=25) x =24;

VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0xF000)>>4)
| ((VideoMemory[80*y+x] & 0x0F00)<<4)
| ((VideoMemory[80*y+x] & 0x00FF));



  }

return esp;
}
