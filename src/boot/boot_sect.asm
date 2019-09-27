[org 0x7c00]
KERNEL_OFFSET equ 0x1000
BIOS_RESERVED equ 0x100000
MEM_SIZE      equ 0x7a12000
	mov ax, 0
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

    mov bp, 0x9000
    mov sp, bp

    call load_kernel
    call switch_to_pm

    jmp $

%include "boot_sect_print.asm"
%include "boot_sect_disk.asm"
%include "gdt.asm"
%include "print_pm.asm"
%include "switch_pm.asm"

[bits 16]
load_kernel:
    mov bx, KERNEL_OFFSET
    mov dh, 50
    call disk_load
    ret

[bits 32]
BEGIN_PM:
    push MEM_SIZE
	call KERNEL_OFFSET
    jmp $

times 510 - ($-$$) db 0
dw 0xaa55
