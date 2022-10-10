#ifndef ACKEROS_DRIVERS_MOUSE_H_
#define ACKEROS_DRIVERS_MOUSE_H_

#include <common/types.h>
#include <hardware/port.h>
#include <drivers/driver.h>
#include <hardware/interrupts.h>

namespace ackeros {
    namespace drivers {
        class MouseEventHandler {
            public:
                MouseEventHandler();

                virtual void OnActivate();
                virtual void OnMouseDown();
                virtual void OnMouseUp();
                virtual void OnMouseMove(int x, int y);
        };

        class MouseDriveer : public ackeros::hardware::InterruptHandler, public Driver {
            ackeros::hardware::Port8Bit dataport;
            ackeros::hardware::Port8Bit cmdport;
            ackeros::common::uint8_t buffer[3];
            ackeros::common::uint8_t offset;
            ackeros::common::uint8_t buttons;

            MouseEventHandler* handler;
            
            public:
                MouseDriver(ackeros::hardware::InterruptManager* manager,
                            MouseEventHandler* handler);
                ~MouseDriver();
                virtual acker::common::uint32_t HandleInterrupt(ackeros::common::uint32_t esp);
                virtual void Activate();
        };
    }
}

#endif
