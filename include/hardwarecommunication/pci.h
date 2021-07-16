#ifndef __MYOS__HARDWARECOMMUNICATION__PCI_H
#define __MYOS__HARDWARECOMMUNICATION__PCI_H

#include <gdt.h>
#include <common/types.h>
#include <drivers/driver.h>
#include <hardwarecommunication/port.h>

namespace myos
{
    namespace hardwarecommunication

    {
        class PeripheralComponentInterconnectDescriptor{
            public:
            myos::common::uint32_t portBase;
            myos::common::uint32_t interrupt;

            myos::common::uint16_t bus;
             myos::common::uint16_t device;
             myos::common::uint16_t function;


              myos::common::uint16_t vendor_id;
               myos::common::uint16_t device_id;


                myos::common::uint8_t class_id;
                myos::common::uint8_t subclass_id;
                myos::common::uint8_t inteface_id;

                 myos::common::uint8_t revision;
                 PeripheralComponentInterconnectDescriptor();
                 ~PeripheralComponentInterconnectDescriptor();
        };
        
        
        
        class PeripheralComponentInterconnectController
        {

            myos::hardwarecommunication::Port32Bit dataPort;
            myos::hardwarecommunication::Port32Bit commandPort;
            public: 
            PeripheralComponentInterconnectController();
            ~PeripheralComponentInterconnectController();

            myos::common::uint32_t Read(myos::common::uint16_t bus, myos::common::uint16_t device, myos::common::uint16_t function, myos::common::uint32_t registeroffset);
            void Write(myos::common::uint16_t bus, myos::common::uint16_t device, myos::common::uint16_t function, myos::common::uint32_t registeroffset, myos::common::uint32_t value);
            bool DeviceHasFunctions(myos::common::uint16_t bus, myos::common::uint16_t device);

            void SelectDrivers(myos::drivers::DriverManager* driverManager);
            PeripheralComponentInterconnectDescriptor GetDeviceDescriptor(myos::common::uint16_t bus, myos::common::uint16_t device, myos::common::uint16_t function);
        };
    }
}

#endif