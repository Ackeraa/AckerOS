#include <common/types.h>
#include <gdt.h>

using namespace ackeros;
using namespace ackeros::common;

void printf(char* str) {
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

    while (1);
}
