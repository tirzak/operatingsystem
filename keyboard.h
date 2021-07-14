#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "types.h"
#include "port.h"
#include "interrupts.h"
#include "driver.h"
class KeyBoardEventHandler{

public:
  KeyBoardEventHandler();
  virtual void OnKeyDown(char);
  virtual void OnKeyUp(char);

};
class KeyboardDriver : public InterruptHandler, public Driver
{
  Port8Bit dataport;
  Port8Bit commandPort;
  KeyBoardEventHandler* handler;

public:
  KeyboardDriver(InterruptManager* manager, KeyBoardEventHandler* handler);
  ~KeyboardDriver();
  virtual uint32_t HandleInterrupt(uint32_t esp);
  virtual void Activate();
};

#endif
