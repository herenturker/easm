section .data
    msg db "Hello, World!", 0   ; Null-terminated string

section .text
    global _start

_start:
    mov edx, 13          ; length of string
    mov ecx, msg         ; pointer to string
    mov ebx, 1           ; file descriptor (stdout)
    mov eax, 4           ; sys_write syscall number
    int 0x80             ; call kernel

    mov eax, 1           ; sys_exit
    xor ebx, ebx         ; exit code 0
    int 0x80             ; call kernel
