#ifndef ACKEROS_HARDWARE_PCI_H_
#define ACKEROS_HARDWARE_PCI_H_

#include <hardware/port.h>
#include <drivers/drivers.h>
#include <common/types.h>
#include <hardware/interrupts.h>

namespace ackeros {
    namespace hardware {
        enum BaseAddressRegisterType {
            MemoryMapping = 0,
            InputOutput = 1
        };

        class BaseAddressRegister {
            public:
                bool prefetchable;
                ackeros::common::uint8_t* address;
                ackeros::common::uint32_t size;
                BaseAddressRegisterType type;
        };

        class PCIDeviceDescriptor {
            public:
                ackeros::common::uint32_t portBase;
                ackeros::common::uint32_t interrupt;

                ackeros::common::uint16_t bus;
                ackeros::common::uint16_t device;
                ackeros::common::uint16_t function;

                ackeros::common::uint16_t vender_id;
                ackeros::common::uint16_t device_id;

                ackeros::common::uint8_t  class_id;
                ackeros::common::uint8_t  subclass_id;
                ackeros::common::uint8_t  interface_id;

                ackeros::common::uint8_t revision;

                PCIDeviceDescriptor();
                ~PCIDeviceDescriptor();
        };

        class PCIController {
            Port32Bit dataPort;
            Port32Bit commandPort;

            public:
                PCIController();
                ~PCIController();

                ackeros::common::uint32_t Read(ackeros::common::uint16_t bus, 
                                               ackeros::common::uint16_t device,
                                               ackeros::common::uint16_t function,
                                               ackeros::common::uint32_t, registeroffset);
                void Write (ackeros::common::uint16_t bus, 
                            ackeros::common::uint16_t device,
                            ackeros::common::uint16_t function,
                            ackeros::common::uint32_t, registeroffset,
                            ackeros::common::uint32_t, value)
                bool DeviceHasFunctions(acker::common::uint16_t bus,
                                        ackeros::common::uint16_t device)
                

                void SelectDrivers(ackeros::drivers::DriverManager* driverManageer,
                                   ackeros::hardware::InterruptManager* interrupts);
                ackeros::drivers::Driver* GetDriver(PCIDeviceDescriptor dev, 
                                                    ackeros::hardware::InterruptManager* interrupts);
                PCIDeviceDescriptor GetDeviceDescriptor(ackeros::common::uint16_t bus,
                                                        ackeros::common::uint16_t device,
                                                        ackeros::common::uint16_t function);
                BaseAddressRegister GetBaseAddressRegister(ackeros::commom::uint16_t bus,
                                                           ackeros::common::uint16_t device,
                                                           ackeros::common::uint16_t function,
                                                           ackeros::common::uint16_t bar);
        };
    }
}

#endif
