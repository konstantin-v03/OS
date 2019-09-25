[bits 16]

switch_to_pm:
    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:init_pm

[bits 32]

init_pm:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000
    mov esp, ebp

    mov edx, 0xFFFFFFFF
    sub edx, BIOS_RESERVED
    mov esi, BIOS_RESERVED
    call probeRAM
    add ecx, BIOS_RESERVED
    and ecx, ~0x00000FFF 
    sub ecx, 0x1000

    mov ebp, ecx
    mov esp, ebp
    call BEGIN_PM
