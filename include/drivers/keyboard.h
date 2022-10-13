#ifndef ACKEROS_DRIVERS_KEYBOARD_H_
#define ACKEROS_DRIVERS_KEYBOARD_H_

#include <common/types.h>
#include <hardware/interrupts.h>
#include <drivers/driver.h>
#include <hardware/port.h>

namespace ackeros {
    namespace drivers {
        class KeyboardEventHandler {
            public:
                KeyboardEventHandler();

                virtual void OnKeyDown(char);
                virtual void OnKeyUp(char);
        };

        class KeyboardDriver : public ackeros::hardware::InterruptHandler, public Driver {
            ackeros::hardware::Port8Bit dataport;
            ackeros::hardware::Port8Bit cmdport;

            KeyboardEventHandler* handler;

            public:
                KeyboardDriver(ackeros::hardware::InterruptManager* manager,
                               KeyboardEventHandler* handler);
            ~KeyboardDriver();
            virtual ackeros::common::uint32_t HandleInterrupt(ackeros::common::uint32_t esp);
            virtual void Activate();
        };
    }
}


#endif
