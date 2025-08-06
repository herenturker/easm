; "Hello, world!" tarzı .asm dosyası

section .text
start:
    mov ax, 0xB800     ; Video belleği adresi
    mov ds, ax

    mov ah, 0x0E       ; BIOS TTY yazma fonksiyonu
    mov al, 'H'
    int 0x10

    mov al, 'i'
    int 0x10

    hlt                ; CPU'yu durdur