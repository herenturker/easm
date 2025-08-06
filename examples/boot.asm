; Simple bootloader test code (x86 real mode, Intel syntax)
BITS 16
ORG 0x7C00               ; Boot sector start address

; Constants using EQU directive
SECTOR_SIZE equ 512
BOOT_MSG_LEN equ 13

; Data segment
section .data
boot_msg db 'Bootloader OK', 0

section .text
start:
    cli                 ; Clear interrupts
    xor ax, ax
    mov ds, ax          ; Setup DS segment to 0
    mov es, ax          ; Setup ES segment to 0

    mov si, boot_msg    ; Point SI to boot message
    mov cx, BOOT_MSG_LEN

print_loop:
    lodsb               ; Load byte at DS:SI into AL, increment SI
    mov ah, 0x0E        ; Teletype output BIOS interrupt
    int 0x10            ; BIOS video interrupt
    loop print_loop     ; Loop CX times

hang:
    jmp hang            ; Infinite loop to halt

times SECTOR_SIZE - ($ - $$) db 0   ; Fill rest of boot sector with zeros
dw 0xAA55                           ; Boot sector signature
