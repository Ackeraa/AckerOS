#ifndef ACKEROS_HARDWARE_INTEERRUPTS_H_
#define ACKEROS_HARDWARE_INTEERRUPTS_H_

#include <gdt.h>
#include <common/types.h>
#include <hardware/port.h>

namespace ackeros {
    namespace hardware {
        class InterruptManager;
        
        class InterruptHandler {
            protected:
                ackerps::common::uint8_t InterruptNumber;
                InterruptManager* interruptManager;
                
                InterruptHandler(InterruptManager* interruptManager,
                                 ackeros::common::uint8_t InterruptNumber);
                ~InterruptHandler();

            public:
                virtual ackeros::common::uint32_t HandleInterrupt(ackeros::common::uint32_t esp);
        };

        class InterruptManager {
            friend class InterruptHandler;

            protected:
                static InterruptManager* ActiveInterruptManager;
                InterruptHandler* handlers[256];
                
                struct GateDescriptor {
                    ackeros::common::uint16_t handlerAddressLowBits;
                    ackeros::common::uint16_t gdt_codeSegmentSelector;
                    ackeros::common::uint8_t reserved;
                    ackeros::common::uint16_t handlerAddressHighBits;
                } __attribute__((packed));

                static GateDescriptor interruptDescriptorTable[256];

                struct InterruptDescriptorTablePointer {
                    ackeros::common::uint16_t size;
                    ackeros::common::uint32_t base;
                } __attribute__((packed));

                ackeros::common::uint16_t hardwareInterruptOffset;
                static void setInterruptDescriptorTableEntry(ackeros::common::uint8_t interrupt,
                                                            ackeros::common::uint16_t codeSegmentSelectorOffset, void (*hhandler)(),
                                                            ackeros::common::uint8_t DescriptorPrivilegeLevel,
                                                            ackeros::common::uint8_t DescriptorType);
                
                static void InterruptIgnore();

                static void HandleInterruptRequest0x00();
                static void HandleInterruptRequest0x01();
                static void HandleInterruptRequest0x02();
                static void HandleInterruptRequest0x03();
                static void HandleInterruptRequest0x04();
                static void HandleInterruptRequest0x05();
                static void HandleInterruptRequest0x06();
                static void HandleInterruptRequest0x07();
                static void HandleInterruptRequest0x08();
                static void HandleInterruptRequest0x09();
                static void HandleInterruptRequest0x0A();
                static void HandleInterruptRequest0x0B();
                static void HandleInterruptRequest0x0C();
                static void HandleInterruptRequest0x0D();
                static void HandleInterruptRequest0x0E();
                static void HandleInterruptRequest0x0F();
                static void HandleInterruptRequest0x31();

                static void HandleInterruptRequest0x80();

                static void HandleException0x00();
                static void HandleException0x01();
                static void HandleException0x02();
                static void HandleException0x03();
                static void HandleException0x04();
                static void HandleException0x05();
                static void HandleException0x06();
                static void HandleException0x07();
                static void HandleException0x08();
                static void HandleException0x09();
                static void HandleException0x0A();
                static void HandleException0x0B();
                static void HandleException0x0C();
                static void HandleException0x0D();
                static void HandleException0x0E();
                static void HandleException0x0F();
                static void HandleException0x10();
                static void HandleException0x11();
                static void HandleException0x12();
                static void HandleException0x13();

                static ackeros::common::uint32_t HandleInterrupt(ackeros::common::uint8_t interrupt,
                                                                 ackeros::common::uint32_t esp);

                static ackeros::common::uint32_t DoHandleInterrupt(ackeros::common::uint8_t interrupt,
                                                                 ackeros::common::uint32_t esp);
                
            public:
                InterruptManager(ackeros::common::uint16_t hardwareInterruptOffset, 
                                 ackeros::GlobalDescriptorTable* GlobalDescriptorTable);
                ~InterruptManager();

                ackeros::common::uint16_t HardwareInterruptOffset();
                void Activate();
                void Deactivate();
        };
    }
}

#endif
