#include <hardware/interrupts.h>
using namespace ackeros;
using namespace ackeros::common;
using namespace ackeros::hardware;

void printf(const char* str);
void printfHex(uint8_t);

InterruptHandler::InterruptHandler(InterruptManager* interruptManager,
                                   uint8_t InterruptNumber) {
    this->InterruptNumber = InterruptNumber;
    this->interruptManager = interruptManager;
    interruptManager->handlers[InterruptNumber] = this;
}

InterruptHandler::~InterruptHandler() {
    if (interruptManager->handlers[InterruptNumber] == this) 
        interruptManager->handlers[InterruptNumber] = 0;
}

uint32_t InterruptHandler::HandleInterrupt(uint32_t esp) {
    return esp;
}


InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];
InterruptManager* InterruptManager::ActiveInterruptManager = 0;


void InterruptManager::SetInterruptDescriptorTableEntry(uint8_t interrupt,
                                                        uint16_t CodeSegment,
                                                        void (*handler)(),
                                                        uint8_t DescriptorPrivilegeLevel,
                                                        uint8_t DescriptorType) {
    interruptDescriptorTable[interrupt].handlerAddressLowBits = ((uint32_t) handler) & 0xFFFF;
    interruptDescriptorTable[interrupt].handlerAddressHighBits = (((uint32_t) handler) >> 16) & 0xFFFF;
    interruptDescriptorTable[interrupt].gdt_codeSegmentSelector = CodeSegment;

    const uint8_t IDT_DESC_PRESENT = 0x80; // why? 
    interruptDescriptorTable[interrupt].access = IDT_DESC_PRESENT | ((DescriptorPrivilegeLevel & 3) << 5) | DescriptorType;
    interruptDescriptorTable[interrupt].reserved = 0;
}

InterruptManager::InterruptManager(uint16_t hardwareInterruptOffset,
        GlobalDescriptorTable* gdt) 
    : picMasterCmdPort(0x20),
    picMasterDataPort(0x21),
    picSlaveCmdPort(0xA0),
    picSlaveDataPort(0xA1) {

    this->hardwareInterruptOffset = hardwareInterruptOffset;
    uint32_t CodeSegment = gdt->CodeSegmentSelector();

    const uint8_t IDT_INTERRUPT_GATE = 0xE;
    for (uint8_t i = 255; i > 0; --i) {
        SetInterruptDescriptorTableEntry(i, CodeSegment, &InterruptIgnore, 0, IDT_INTERRUPT_GATE);
        handlers[i] = 0;
    }
    SetInterruptDescriptorTableEntry(0, CodeSegment, &InterruptIgnore, 0, IDT_INTERRUPT_GATE);
    handlers[0] = 0;

    // Need to be fixed, maybe.
    SetInterruptDescriptorTableEntry(0x00, CodeSegment, &HandleException0x00, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x01, CodeSegment, &HandleException0x01, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x02, CodeSegment, &HandleException0x02, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x03, CodeSegment, &HandleException0x03, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x04, CodeSegment, &HandleException0x04, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x05, CodeSegment, &HandleException0x05, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x06, CodeSegment, &HandleException0x06, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x07, CodeSegment, &HandleException0x07, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x08, CodeSegment, &HandleException0x08, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x09, CodeSegment, &HandleException0x09, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0A, CodeSegment, &HandleException0x0A, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0B, CodeSegment, &HandleException0x0B, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0C, CodeSegment, &HandleException0x0C, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0D, CodeSegment, &HandleException0x0D, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0E, CodeSegment, &HandleException0x0E, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0F, CodeSegment, &HandleException0x0F, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x10, CodeSegment, &HandleException0x10, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x11, CodeSegment, &HandleException0x11, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x12, CodeSegment, &HandleException0x12, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x13, CodeSegment, &HandleException0x13, 0, IDT_INTERRUPT_GATE);

    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x00, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x01, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x02, CodeSegment, &HandleInterruptRequest0x02, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x03, CodeSegment, &HandleInterruptRequest0x03, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x04, CodeSegment, &HandleInterruptRequest0x04, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x05, CodeSegment, &HandleInterruptRequest0x05, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x06, CodeSegment, &HandleInterruptRequest0x06, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x07, CodeSegment, &HandleInterruptRequest0x07, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x08, CodeSegment, &HandleInterruptRequest0x08, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x09, CodeSegment, &HandleInterruptRequest0x09, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0A, CodeSegment, &HandleInterruptRequest0x0A, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0B, CodeSegment, &HandleInterruptRequest0x0B, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0C, CodeSegment, &HandleInterruptRequest0x0C, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0D, CodeSegment, &HandleInterruptRequest0x0D, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0E, CodeSegment, &HandleInterruptRequest0x0E, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0F, CodeSegment, &HandleInterruptRequest0x0F, 0, IDT_INTERRUPT_GATE);

    SetInterruptDescriptorTableEntry(0x80, CodeSegment, &HandleInterruptRequest0x80, 0, IDT_INTERRUPT_GATE);

    picMasterCmdPort.Write(0x11);
    picSlaveCmdPort.Write(0x11);

    picMasterDataPort.Write(hardwareInterruptOffset);
    picSlaveDataPort.Write(hardwareInterruptOffset + 8);

    picMasterDataPort.Write(0x04);
    picSlaveDataPort.Write(0x02);

    picMasterDataPort.Write(0x01);
    picSlaveDataPort.Write(0x01);

    picMasterDataPort.Write(0x00);
    picSlaveDataPort.Write(0x00);

    InterruptDescriptorTablePointer idt_pointer;
    idt_pointer.size = 256 * sizeof(GateDescriptor) - 1;
    idt_pointer.base = (uint32_t)interruptDescriptorTable;
    asm volatile("lidt %0" : : "m" (idt_pointer));
}

InterruptManager::~InterruptManager() {
    Deactivate();
}

uint16_t InterruptManager::HardwareInterruptOffset() {
    return hardwareInterruptOffset;
}

void InterruptManager::Activate() {
    if (ActiveInterruptManager != 0)
        ActiveInterruptManager->Deactivate();

    ActiveInterruptManager = this;
    asm("sti");
}

void InterruptManager::Deactivate() {
    if (ActiveInterruptManager == this) {
        ActiveInterruptManager = 0;
        asm("cli");
    }
}

uint32_t InterruptManager::HandleInterrupt(uint8_t interrupt, uint32_t esp) {
    if (ActiveInterruptManager != 0) 
        return ActiveInterruptManager->DoHandleInterrupt(interrupt, esp);
    return esp;
}

uint32_t InterruptManager::DoHandleInterrupt(uint8_t interrupt, uint32_t esp) {
    if (handlers[interrupt] != 0) {
        esp = handlers[interrupt]->HandleInterrupt(esp);
    } else if (interrupt != hardwareInterruptOffset) {
        printf("UNHANDLED INTERRUPT 0x");
        printfHex(interrupt);
    } 

    if (hardwareInterruptOffset <= interrupt && interrupt < hardwareInterruptOffset + 16) {
        picMasterCmdPort.Write(0x20);
        if (hardwareInterruptOffset + 8 <= interrupt) 
            picSlaveCmdPort.Write(0x20);
    }

    return esp;
}
