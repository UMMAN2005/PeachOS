#include "kernel.h"
#include <stddef.h>
#include <stdint.h>
#include "idt/idt.h"
#include "io/io.h"


uint16_t* video_mem = 0;
uint16_t terminal_row;
uint16_t terminal_col;

uint16_t terminal_make_char(char character, char color) {
    return (color << 8) | character;
}

void terminal_putchar(int x, int y, char character, char color) {
    video_mem[y * VGA_WIDTH + x] = terminal_make_char(character, color);
}

void terminal_writechar(char character, char color) {
    if (character == '\n') {
        terminal_col = 0;
        terminal_row++;
        return;
    } else if (character == '\t') {
        terminal_col += 4;
        return;
    }
    terminal_putchar(terminal_col, terminal_row, character, color);
    terminal_col++;
    if (terminal_col >= VGA_WIDTH) {
        terminal_col = 0;
        terminal_row++;
    }
}

void terminal_initialize() {
    video_mem = (uint16_t*)(0xB8000);
    terminal_row = 0;
    terminal_col = 0;
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            terminal_putchar(x, y, ' ', 0);
        }
    }
}

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

void print(const char* str) {
    size_t len = strlen(str);
    for (int i = 0; i < len; i++) {
        terminal_writechar(str[i], 15);
    }
}

void kernel_main() {
    terminal_initialize();
    // video_mem[0] = 0x0341;  // Color : ASCII on hex
    print("Hello,World!");

    // Initialize the Interrupt Descriptor Table
    idt_init();

    // Enable the system interrupts
    enable_interrupts();
}