;If try to remove comments that print messages, it won't run on real hardware

[org 0x7c00]
KERNEL_OFFSET equ 0x1000
	mov ax, 0
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

    mov bp, 0x9000
    mov sp, bp

    ;mov bx, MSG_REAL_MODE 
    ;call print
    ;call print_nl
    call load_kernel

	;mov bx, MSG_LOADED_KERNEL
	;call print
	;call print_nl

    call switch_to_pm

    jmp $

%include "boot_sect_print.asm"
%include "boot_sect_disk.asm"
%include "gdt.asm"
%include "print_pm.asm"
%include "switch_pm.asm"

[bits 16]
load_kernel:
    ;mov bx, MSG_LOADING_KERNEL
    ;call print
    ;call print_nl
    mov bx, KERNEL_OFFSET
    mov dh, 30
    call disk_load
    ret

[bits 32]
BEGIN_PM:
    ;mov ebx, MSG_PROT_MODE
    ;call print_string_pm
	call KERNEL_OFFSET
    jmp $

MSG_REAL_MODE db "Started in 16-bit Real Mode", 0
MSG_LOADING_KERNEL db "Loading kernel into memory", 0
MSG_LOADED_KERNEL db "Kernel was successfully loaded", 0
MSG_PROT_MODE db "Loaded 32-bit protected mode", 0

times 510 - ($-$$) db 0
dw 0xaa55
