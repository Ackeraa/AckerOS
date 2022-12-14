#include <common/types.h>
#include <gdt.h>
#include <hardware/interrupts.h>
#include <hardware/pci.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>

using namespace ackeros;
using namespace ackeros::common;
using namespace ackeros::drivers;
using namespace ackeros::hardware;

void printf(const char* str) {
    uint16_t* VideoMemory = (uint16_t*)0xb8000;

    static uint8_t x = 0, y = 0;

    for (int i = 0; str[i] != '\0'; ++i) {
        if (str[i] == '\n') {
            x = 0;
            y++;
        } else {
            VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | str[i];
            x++;
        }
        
        // new line
        if (x >= 80) {
            x = 0;
            y++;
        }
        
        // refresh
        if (y >= 25) {
            for (y = 0; y < 25; ++y) 
                for (x = 0; x < 80; ++x)
                    VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
        }
    }

}

void printfHex(uint8_t key) {
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    printf(foo);
}

void printfHex16(uint16_t key) {
    printfHex((key >> 8) & 0xFF);
    printfHex(key & 0xFF);
}

void printfHex32(uint32_t key) {
    printfHex16((key >> 16) & 0xFFFF);
    printfHex16(key & 0xFFFF);
}

class PrintfKeyboardEventHandler : public KeyboardEventHandler {
    public:
        void OnKeyDown(char c) {
            char* foo = " ";
            foo[0] = c;
            printf(foo);
        }
};

class MouseToConsole : public MouseEventHandler {
    int8_t x, y;
    public:
        MouseToConsole() {
            uint16_t* VideoMemory = (uint16_t*)0xb8000;
            x = 40;
            y = 12;
            VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0x0F00) << 4
                                    | (VideoMemory[80 * y + x] & 0xF000) >> 4
                                    | (VideoMemory[80 * y + x] & 0x00F);
        }

        virtual void OnMouseMove(int xoffset, int yoffset) {
            static uint16_t* VideoMemory = (uint16_t*)0xb8000;
            VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0x0F00) << 4
                                    | (VideoMemory[80 * y + x] & 0xF000) >> 4
                                    | (VideoMemory[80 * y + x] & 0x00F);

            x += xoffset;
            if (x >= 80) x = 79;
            if (x < 0) x = 0;
            y += yoffset;
            if (y >= 25) y = 24;
            if (y < 0) y = 0;

            VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0x0F00) << 4
                                    | (VideoMemory[80 * y + x] & 0xF000) >> 4
                                    | (VideoMemory[80 * y + x] & 0x00F);
        }
};

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors() {
    for (constructor* i = &start_ctors; i != &end_ctors; ++i) {
        (*i)();
    }
}


extern "C" void kernelMain(void* multiboot_structure, unsigned int magicnumber) {
    printf("Hello world!");

    GlobalDescriptorTable gdt;

    InterruptManager interrupts(0x20, &gdt);
    
    printf("Initializing Hardware, Stage 1\n");

    DriverManager drvManager;
    PrintfKeyboardEventHandler kbhandler;
    KeyboardDriver keyboard(&interrupts, &kbhandler);
    drvManager.AddDriver(&keyboard);
    
    MouseToConsole mshandler;
    MouseDriver mouse(&interrupts, &mshandler);
    drvManager.AddDriver(&mouse);

    PCIController pciController;
    pciController.SelectDrivers(&drvManager, &interrupts);

    printf("Initializing Hardware, Stage 2\n");
    drvManager.ActivateAll();

    printf("Initializing Hardware, Stage 3\n");

    interrupts.Activate();

    printf("\n\n\n\n");

    while (1);
}
