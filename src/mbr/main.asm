org 0x7C00

start:
    ; start vga poop mode
    mov ah, 0x00
    mov al, 0x13
    int 0x10
    mov ax, 0xA000
    mov es, ax

%macro sleep 2
    ; credits to memz cuz i got no skill for asm
    push dx
    mov ah, 86h
    mov cx, %1
    mov dx, %2
    int 15h
    pop dx
%endmacro

main_loop:
    mov di, 0
    mov cx, 320*200

red_filler:
    mov al, 4 ; 4 is red
    stosb
    loop red_filler

    sleep 2,0

    mov di,0
    mov cx, 320*200

black_filler:
    mov al, 0 ; 0 is black
    stosb
    loop black_filler

    sleep 2,0

    mov di,0
    mov cx, 320*200

    jmp main_loop

times 510 - ($ - $$) db 0
dw 0xAA55
