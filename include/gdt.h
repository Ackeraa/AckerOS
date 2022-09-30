#ifndef __ACKEROS__GDT_H
#define __ACKEROS__GDT_H

#include <common/types.h>

namespace ackeros {

    class GlobalDescriptorTable {
        public:
            class SegmentDescriptor {
                private:
                    ackeros::common::uint16_t limit_lo;
                    ackeros::common::uint16_t base_lo;
                    ackeros::common::uint8_t base_hi;
                    ackeros::common::uint8_t type;
                    ackeros::common::uint8_t limit_hi;
                    ackeros::common::uint8_t base_vhi;

                public:
                    SegmentDescriptor(ackeros::common::uint32_t base, ackeros::common::uint32_t limit, ackeros::common::uint8_t type);
                    ackeros::common::uint32_t Base();
                    ackeros::common::uint32_t Limit();
            } __attribute__((packed));

        private:
            SegmentDescriptor nullSegmentSelector;
            SegmentDescriptor unusedSegmentSelector;
            SegmentDescriptor codeSegmentSelector;
            SegmentDescriptor dataSegmentSelector;

        public:
            GlobalDescriptorTable();
            ~GlobalDescriptorTable();

            ackeros::common::uint16_t CodeSegmentSelector();
            ackeros::common::uint16_t DataSegmentSelector();
    };
}

#endif
