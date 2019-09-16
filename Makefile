C_SRCS = $(wildcard $(KERNEL_DIR)*.c) $(wildcard $(DRIVERS_DIR)*.c) $(wildcard $(LIBS_DIR)*.c) $(wildcard $(INT_DIR)*.c)

HDRS = $(wildcard $(KERNEL_DIR)*.h) $(wildcard $(DRIVERS_DIR)*.h) $(wildcard $(LIBS_DIR)*.h) $(wildcard $(INT_DIR)*.h)

OBJS = $(C_SRCS:%.c=%.o) $(INT_DIR)interrupts.o $(KERNEL_DIR)kernel_entry.o

BOOT_DIR = src/boot/
KERNEL_DIR = src/kernel/
DRIVERS_DIR = src/drivers/
INT_DIR = src/interrupts/
LIBS_DIR = src/libs/
BIN_DIR = bin/

CC = gcc
CFLAGS = -g -ffreestanding -Wall -Wextra -fno-exceptions -m32 -fno-pie

os-image.bin: $(BIN_DIR)boot_sect.bin $(BIN_DIR)kernel.bin
	cat $^ > os-image.bin

kernel.elf: $(OBJS)
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ 

$(BIN_DIR)boot_sect.bin: $(BOOT_DIR)*.asm
	nasm -I $(BOOT_DIR) $(BOOT_DIR)boot_sect.asm -f bin -o $@

$(BIN_DIR)kernel.bin: $(OBJS)
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

clean:
	rm -rf $(BIN_DIR)*.bin os-image.bin
	rm -rf $(KERNEL_DIR)*.o $(DRIVERS_DIR)*.o $(INT_DIR)*.o $(LIBS_DIR)*.o $(BOOT_DIR)*.o
