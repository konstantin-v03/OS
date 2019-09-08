BOOT_DIR=src/boot/
KERNEL_DIR=src/kernel/
OBJS_DIR=objs/
BIN_DIR=bin/

os-image.bin: $(BIN_DIR)boot_sect.bin $(BIN_DIR)kernel.bin
	cat $^ > os-image.bin

$(OBJS_DIR)kernel.o: $(KERNEL_DIR)kernel.c
	gcc -m32 -fno-pie -ffreestanding -c $< -o $@

$(OBJS_DIR)k_print.o: $(KERNEL_DIR)k_print.c
	gcc -m32 -fno-pie -ffreestanding -c $< -o $@

$(OBJS_DIR)kernel_entry.o: $(KERNEL_DIR)kernel_entry.asm
	nasm $< -f elf -o $@

$(BIN_DIR)boot_sect.bin: $(BOOT_DIR)*.asm
	nasm -I $(BOOT_DIR) $(BOOT_DIR)boot_sect.asm -f bin -o $@

$(BIN_DIR)kernel.bin: $(OBJS_DIR)kernel_entry.o $(OBJS_DIR)kernel.o $(OBJS_DIR)k_print.o
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

clean:
	rm -rf $(OBJS_DIR)*.o $(BIN_DIR)*.bin
