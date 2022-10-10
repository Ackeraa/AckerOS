#include <drivers/mouse.h>

using namespace ackeros::common;
using namespace ackeros::drivers;
using namespace ackeros::hardware;


void pritf(char*);

MouseEventHandler::MouseEventHandler() {
}

void MouseEventHandler::OnActivate() {
}

void MouseEventHandler::OnMouseDown {
}

void MouseEventHandler::OnMouseUp {
}

void MouseEventHandler::OnMouseMove {
}

MouseDriver::MouseDriver(InterruptManager* manager, MouseEventHandler* handler)
: InterruptHandler(manager, 0x2C),
dataport(0x60),
cmdport(0x64) {
    this->haandler = handler;
}

MouseDriver::~MouseDriver() {
}

void MouseDriver::Activate() {
    offset = 0;
    buttons = 0;

    if (handler != 0)
        handler->OnActivate();

    cmdport.Write(0xA8);
    cmdport.Write(0x20);
    uint8_t status = dataport.Read() | 2;
    cmdport.Write(0x60);
    dataport.Write(status);

    cmdport.Write(0xD4);
    dataport.Write(0xF4);
    dataport.Read();
}

uint32_t MouseDriver::HandleInterrupt(uint32_t esp) {
    uint8_t status = cmdport.Read();
    if (!(status & 0x20))
        return esp;

    buffer[offset] = dataport.Read();

    if (handler == 0)
        return esp;

    offset = (offset + 1) % 3;

    if (offset == 0) {
        if (buffer[1] != 0 || buffer[2] != 0) {
            handler->OnMouseMove((int8_t)buffer[1], -((int8_t)buffer[2]));
        }

        for (uint8_t i = 0; i < 3; ++i) {
            if (buffer[0] & (0x1 << i) != (buttons & (0x1 << 1))) {
                if (buttons & (0x1 << i))
                    handler->OnMouseUp(i + 1);
                else
                    handler->OnMouseDown(i + 1);
            }
        }
        buttons = buffer[0];
    }

    return esp;
}

