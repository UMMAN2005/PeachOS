#ifndef IDT_H
#define IDT_H

#include <stdint.h>

// Structure representing an IDT descriptor
struct idt_desc {
    uint16_t offset_1;  // Offset bits 0 - 15
    uint16_t selector;  // Selector in the GDT
    uint8_t zero;       // Reserved, set to 0
    uint8_t type_attr;  // Type and attributes
    uint16_t offset_2;  // Offset bits 16-31
} __attribute__((packed));

// Structure representing the IDTR descriptor
struct idtr_desc {
    uint16_t limit; // Size of IDT - 1
    uint32_t base;  // Base address of the IDT
} __attribute__((packed));

void idt_init();
void enable_interrupts();
void disable_interrupts();

#endif // IDT_H
