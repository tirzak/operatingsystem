#include <hardwarecommunication/pci.h>
using namespace myos::common;
using namespace myos::hardwarecommunication;


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
void PeripheralComponentInterconnectController::SelectDrivers(drivers::DriverManager *driverManager)
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
                   break;
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