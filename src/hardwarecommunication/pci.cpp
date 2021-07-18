#include <hardwarecommunication/pci.h>
using namespace myos::common;
using namespace myos::hardwarecommunication;
using namespace myos::drivers;

PeripheralComponentInterconnectDescriptor::PeripheralComponentInterconnectDescriptor(){

}
PeripheralComponentInterconnectDescriptor::~PeripheralComponentInterconnectDescriptor(){

}

PeripheralComponentInterconnectController::PeripheralComponentInterconnectController()
    : dataPort(0xCFC),
      commandPort(0xCF8)
{
}
PeripheralComponentInterconnectController::~PeripheralComponentInterconnectController()
{
}

uint32_t PeripheralComponentInterconnectController::Read(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset)
{
    uint32_t id =
        0x1 << 31 | ((bus & 0xFF) << 16) | ((device & 0x1F) << 11) | ((function & 0x07) << 8) | ((registeroffset & 0xFC));

    commandPort.Write(id);
    uint32_t result = dataPort.Read();
    return result >> (8 * (registeroffset % 4));
}
void PeripheralComponentInterconnectController::Write(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset, uint32_t value)
{

    uint32_t id =
        0x1 << 31 | ((bus & 0xFF) << 16) | ((device & 0x1F) << 11) | ((function & 0x07) << 8) | ((registeroffset & 0xFC));
    commandPort.Write(id);
    dataPort.Write(value);
}

bool PeripheralComponentInterconnectController::DeviceHasFunctions(uint16_t bus, uint16_t device)
{
    return Read(bus, device, 0, 0x0E) & (1 << 7);
}
void printsf(const char* str);

void printHex(uint8_t key);
void PeripheralComponentInterconnectController::SelectDrivers(DriverManager *driverManager,  InterruptManager* interruptManager)
{
    for (int bus = 0; bus < 8; bus++)
    {
        for (int device = 0; device < 32; device++)
        {
            int numFunctions = DeviceHasFunctions(bus, device) ? 8 : 1;
            for (int function = 0; function < numFunctions; function++)
            {

               PeripheralComponentInterconnectDescriptor dev = GetDeviceDescriptor(bus, device, function);

               if(dev.vendor_id == 0x0000 || dev.vendor_id == 0xFFFF){
                   continue;
               }

               for(int barNum = 0; barNum < 6; barNum++){
                   BaseAddressRegister bar = GetBaseAddressRegister(bus, device, function, barNum);
                   if(bar.address && (bar.type == InputOutput) )
                        dev.portBase = (uint32_t)bar.address;

                    Driver* drivers = GetDriver(dev, interruptManager);
                    if(drivers != 0){
                        driverManager ->AddDriver(drivers);
                    }
               }

               printsf("PCI BUS ");
               printHex(bus & 0xFF);

              printsf(", DEVICE ");
               printHex(device & 0xFF);

             printsf(", FUNCTION ");
               printHex(function & 0xFF);

             printsf(" = VENDOR ");
               printHex((dev.vendor_id & 0xFF00) >> 8);  
               printHex(dev.vendor_id & 0xFF);  

             printsf(", DEVICE ");
                     printHex((dev.device_id & 0xFF00) >> 8);  
               printHex(dev.device_id & 0xFF);  
             printsf("\n");
            }
        }
    }
}



BaseAddressRegister PeripheralComponentInterconnectController::GetBaseAddressRegister(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar){

BaseAddressRegister result;

uint32_t headertype = Read(bus, device, function, 0x0E) & 0x7F;

int maxBARs = 6 - (4*headertype);
if(bar >= maxBARs)
    return result;


uint32_t bar_value = Read(bus, device, function, 0x10+4*bar);
result.type = (bar_value & 0x1) ? InputOutput : MemoryMapping;
uint32_t temp;


if(result.type == MemoryMapping){
    
    switch ((bar_value >> 1) & 0x3)
    {
    case 0:
        break;
    case 1:
        break;
    case 2: 
        break;
    default:
        break;
    }

}
else{
    result.address = (uint8_t*)(bar_value & ~0x3);
    result.prefetchable= false;

}





return result;
}


 Driver* PeripheralComponentInterconnectController::GetDriver(PeripheralComponentInterconnectDescriptor dev, InterruptManager* interruptManager)
{
    switch (dev.vendor_id)
    {
    case 0x1022://AMD
        switch (dev.device_id)
        {
        case 0x2000:
        printsf("AMD am79c973 ");
            break;
 
        }
        break;
    case 0x8086: //Intel
        break;

    
    }
    switch (dev.class_id)
    {
    case 0x03:  //graphics
         switch (dev.subclass_id)
        {
        case 0x00:
         printsf("VGA ");
            break;
 
        }
        break;
    
    }

 return 0;
}
PeripheralComponentInterconnectDescriptor PeripheralComponentInterconnectController::GetDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t function){
    PeripheralComponentInterconnectDescriptor result;

    result.bus = bus;
    result.device = device;
    result.function = function;
    
    result.vendor_id= Read(bus,device,function, 0x00);
    result.device_id= Read(bus,device,function, 0x02);

    result.class_id= Read(bus,device,function, 0x0b);
    result.subclass_id = Read(bus,device,function, 0x0a);
    result.inteface_id = Read(bus,device,function, 0x09);

    result.revision = Read(bus,device,function, 0x08);
    result.interrupt= Read(bus,device,function, 0x3c);

    return result;

}