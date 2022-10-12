#ifndef ACKEROS_HARDWARE_PORT_H_
#define ACKEROS_HARDWARE_PORT_H_

#include<common/types.h>

namespace ackeros {
    namespace hardware {

        // FIXME: maybe use template to simplify
        class Port {
            protected:
                Port(ackeros::common::uint16_t portnumber);
                ~Port();
                ackeros::common::uint16_t portnumber;
        };

        class Port8Bit : public Port {
            public:
                Port8Bit(ackeros::common::uint16_t portnumber);
                ~Port8Bit();

                virtual ackeros::common::uint8_t Read();
                virtual void Write(ackeros::common::uint8_t data);

            protected:
                static inline ackeros::common::uint8_t Read8(ackeros::common::uint16_t _port) {
                    ackeros::common::uint8_t result;
                    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (_port));

                    return result;
                }

                static inline void Write8(ackeros::common::uint16_t _port, ackeros::common::uint8_t _data) {
                    __asm__ volatile("outb %0, %1" : : "a" (_data), "Nd" (_port));
                }
        };

        class Port8BitSlow : public Port8Bit {
            public:
                Port8BitSlow(ackeros::common::uint16_t portnumber);
                ~Port8BitSlow();

                virtual void Write(ackeros::common::uint8_t data);

            protected:
                static inline void Write8Slow(ackeros::common::uint16_t _port, ackeros::common::uint8_t _data) {
                    __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (_data), "Nd" (_port));
                }
        };

        class Port16Bit : public Port {
            public:
                Port16Bit(ackeros::common::uint16_t portnumber);
                ~Port16Bit();

                virtual ackeros::common::uint16_t Read();
                virtual void Write(ackeros::common::uint16_t data);

            protected:
                static inline ackeros::common::uint16_t Read16(ackeros::common::uint16_t _port) {
                    ackeros::common::uint16_t result;
                    __asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (_port));

                    return result;
                }

                static inline void Write16(ackeros::common::uint16_t _port, ackeros::common::uint16_t _data) {
                    __asm__ volatile("outw %0, %1" : : "a" (_data), "Nd" (_port));
                }
        };

        class Port32Bit : public Port {
            public:
                Port32Bit(ackeros::common::uint16_t portnumber);
                ~Port32Bit();

                virtual ackeros::common::uint32_t Read();
                virtual void Write(ackeros::common::uint32_t data);

            protected:
                static inline ackeros::common::uint32_t Read32(ackeros::common::uint16_t _port) {
                    ackeros::common::uint32_t result;
                    __asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (_port));

                    return result;
                }

                static inline void Write32(ackeros::common::uint16_t _port, ackeros::common::uint32_t _data) {
                    __asm__ volatile("outl %0, %1" : : "a" (_data), "Nd" (_port));
                }
        };
    }
}

#endif




