%include "defs.h"

global _start
global bf_exit
global bf_mmap
global bf_munmap
global bf_open
global bf_close
global bf_fstat
global bf_read
global bf_write

section .text

_start:
    pop rcx
    cmp rcx, 2
    jge bf_pass_arg

    mov rdi, BF_NULL
    call _bf_call_main

_bf_pass_arg:
    mov rdi, [rsp + 8]

_bf_call_main:
    call bf_main
    mov rdi, BF_EXIT_SUCCESS
    call bf_exit

bf_exit:
    mov rax, 60
    syscall

bf_mmap:
    mov rax, 9
    mov r8, rsi         ; fd
    mov rsi, rdi        ; size
    mov rdi, BF_NULL    ; addr
    mov rdx, 1          ; PROT_READ
    mov r10, 0x02       ; MAP_PRIVATE
    xor r9, r9          ; offset

    syscall
    ret

bf_munmap:
    mov rax, 11

    syscall
    ret

bf_open:
    mov rax, 2
    mov rsi, 0   ; O_RDONLY
    xor edx, edx ; mode

    syscall
    ret

bf_close:
    mov rax, 3
    syscall
    ret

bf_fstat:
    mov rax, 4
    syscall
    ret

bf_read:
    push    rbp
    mov     rbp, rsp
    sub     rsp, 16

    mov     byte [rbp-1], 0

    mov     rax, 0
    mov     rdi, 0          ; fd = 0 (stdin)
    lea     rsi, [rbp-1]    ; buf
    mov     rdx, 1          ; count
    syscall

    cmp     rax, 1
    je      _bf_read_success

    mov     eax, -1
    jmp     _bf_readend

_bf_read_success:
    movzx   eax, byte [rbp-1]
    jmp     _bf_readend

_bf_readend:
    leave
    ret

bf_write:
    mov rax, 1
    mov rsi, rdi ; buf
    mov rdi, 1   ; stdout
    mov rdx, 1   ; count
    syscall
    ret
