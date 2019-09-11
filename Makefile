KERNEL_SRCS = $(wildcard $(KERNEL_DIR)*.c)
KERNEL_HDRS = $(wildcard $(KERNEL_DIR)*.h)

DRIVERS_SRCS = $(wildcard $(DRIVERS_DIR)*.c)
DRIVERS_HDRS = $(wildcard $(DRIVERS_DIR)*.h)

LIBS_SRCS = $(wildcard $(LIBS_DIR)*.c)
LIBS_HDRS = $(wildcard $(LIBS_DIR)*.h)

KERNEL_SRCS_NOT_DIR = $(KERNEL_SRCS:$(KERNEL_DIR)%=%)
KERNEL_C_OBJS = $(KERNEL_SRCS_NOT_DIR:%.c=%.o)

DRIVERS_SRCS_NOT_DIR = $(DRIVERS_SRCS:$(DRIVERS_DIR)%=%)
DRIVERS_C_OBJS = $(DRIVERS_SRCS_NOT_DIR:%.c=%.o)

LIBS_SRCS_NOT_DIR = $(LIBS_SRCS:$(LIBS_DIR)%=%)
LIBS_C_OBJS = $(LIBS_SRCS_NOT_DIR:%.c=%.o)

HDRS = $(KERNEL_HDRS) $(DRIVERS_HDRS) $(LIBS_HDRS)
OBJS = $(DRIVERS_C_OBJS:%=$(OBJS_DIR)%) $(KERNEL_C_OBJS:%=$(OBJS_DIR)%) $(LIBS_C_OBJS:%=$(OBJS_DIR)%)

BOOT_DIR = src/boot/
KERNEL_DIR = src/kernel/
DRIVERS_DIR = src/drivers/
LIBS_DIR = src/libs/
OBJS_DIR = objs/
BIN_DIR = bin/

os-image.bin: $(BIN_DIR)boot_sect.bin $(BIN_DIR)kernel.bin
	cat $^ > os-image.bin

kernel.elf: $(OBJS_DIR)kernel_entry.o ${OBJS}
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ 

$(BIN_DIR)boot_sect.bin: $(BOOT_DIR)*.asm
	nasm -I $(BOOT_DIR) $(BOOT_DIR)boot_sect.asm -f bin -o $@

$(BIN_DIR)kernel.bin: $(OBJS_DIR)kernel_entry.o $(OBJS)
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

$(OBJS_DIR)%.o: $(DRIVERS_DIR)%.c $(HDRS)
	gcc -m32 -fno-pie -ffreestanding -c $< -o $@

$(OBJS_DIR)%.o: $(KERNEL_DIR)%.c $(HDRS)
	gcc -m32 -fno-pie -ffreestanding -c $< -o $@

$(OBJS_DIR)%.o: $(LIBS_DIR)%.c $(HDRS)
	gcc -m32 -fno-pie -ffreestanding -c $< -o $@

$(OBJS_DIR)%.o: $(KERNEL_DIR)%.asm
	nasm $< -f elf -o $@

clean:
	rm -rf $(OBJS_DIR)*.o $(BIN_DIR)*.bin
