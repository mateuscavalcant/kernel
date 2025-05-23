CC = i686-elf-gcc
ASM = nasm
LD = i686-elf-ld

CFLAGS = -ffreestanding -O2 -Wall -Wextra -nostdlib
ASMFLAGS = -f elf32
LDFLAGS = -T linker.ld

OBJS = boot.o kernel.o main.o

kernel.bin: $(OBJS)
    $(LD) $(LDFLAGS) -o $@ $^

boot.o: boot.asm
    $(ASM) $(ASMFLAGS) -o $@ $<

%.o: %.c
    $(CC) $(CFLAGS) -c -o $@ $<

clean:
    rm -f *.o kernel.bin

.PHONY: clean
