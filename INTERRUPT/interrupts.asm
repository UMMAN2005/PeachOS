ORG 0
BITS 16

_start:
    jmp short start
    nop

times 33 db 0

start:
    jmp 0x7c0:next_step

handle_zero:
    mov ah, 0eh
    mov al, '0'
    mov bx, 0
    int 0x10
    iret

handle_one:
    mov ah, 0eh
    mov al, '1'
    mov bx, 0
    int 0x10
    iret

next_step:
    cli ; Clear Interrupts
    mov ax, 0x7c0
    mov ds, ax
    mov es, ax
    mov ax, 0x00
    mov ss, ax
    mov sp, 0x7c00
    sti ; Enables Interrupts

    ; Change int 0 to call handle_zero
    mov word[ss:0x00], handle_zero
    mov word[ss:0x02], 0x7c0

    ; Change int 1 to call handle_one
    mov word[ss:0x04], handle_one
    mov word[ss:0x06], 0x7c0

    ; Executing int 0 with division
    mov ax, 0
    div ax

    ; Executing int 0 and int 1 with just calling
    int 0
    int 1

    jmp $

print:
    mov bx, 0
.loop:
    lodsb
    cmp al, 0
    je .done
    call print_char
    jmp .loop
.done:
    ret

print_char:
    mov ah, 0eh
    int 0x10
    ret

times 510-($ - $$) db 0
dw 0xAA55