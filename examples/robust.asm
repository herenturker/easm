BITS 16
ORG 0x7C00

MAXLEN equ 64
msg db "Hello, EASM!", 0

section .text
start:
    xor ax, ax
    mov ds, ax
    mov si, msg
    mov cx, MAXLEN

.loop:
    lodsb
    cmp al, 0
    je done
    mov ah, 0x0E
    int 0x10
    loop .loop

done:
    hlt

times 510 - ($ - $$) db 0
dw 0xAA55
