#include "idt.h"
#include "kernel.h"
#include "config.h"
#include <stdint.h>
#include "memory/memory.h"
#include "io/io.h"

// Define the array to hold IDT descriptors
struct idt_desc idt_descriptors[PEACHOS_TOTAL_INTERRUPTS];

// Define the IDTR descriptor
struct idtr_desc idtr_descriptor;

// External function to load the IDT, assembly implementation
extern void idt_load(struct idtr_desc* ptr);
extern void int21h();
extern void no_interrupt();

void int21h_handler() {
    print("Keyboard pressed!\n");
    outb(0x20, 0x20);
}

void no_interrupt_handler() {
    outb(0x20, 0x20);
}

// Define interrupt handler functions
void idt_zero() {
    print("Divide by zero error!\n");
}

// Set up an IDT descriptor
void idt_set(int interrupt_no, void* address) {
    struct idt_desc* desc = &idt_descriptors[interrupt_no];
    desc->offset_1 = (uint32_t)address & 0x0000ffff;
    desc->selector = KERNEL_CODE_SELECTOR;
    desc->zero = 0x00;
    desc->type_attr = 0xEE;
    desc->offset_2 = (uint32_t)address >> 16;
}

// Initialize the IDT
void idt_init() {
    // Clear the IDT descriptors array
    memset(idt_descriptors, 0, sizeof(idt_descriptors));

    // Set up the IDTR descriptor
    idtr_descriptor.limit = sizeof(idt_descriptors) - 1;
    idtr_descriptor.base = (uint32_t)idt_descriptors;

    // Set up interrupt handlers
    for (int i = 0; i < PEACHOS_TOTAL_INTERRUPTS; i++) {
        idt_set(i, no_interrupt);
    }
    idt_set(0, idt_zero);
    idt_set(0x21, int21h);

    // Load the interrupt descriptor table
    idt_load(&idtr_descriptor);
}
