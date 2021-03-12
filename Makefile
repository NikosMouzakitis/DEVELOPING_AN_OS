OBJECTS= loader.o io.o x86.o framebuffer.o serial.o strlib.o kmain.o
#OBJECTS= loader.o io.o x86int.o x86.o framebuffer.o serial.o strlib.o kmain.o

CC= gcc

CFLAGS= -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
	-nostartfiles -nodefaultlibs -Wall -Wextra -Wno-error -Wno-unused-variable \
        -c

LDFLAGS= -T link.ld -melf_i386

AS= nasm

ASFLAGS= -f elf

all: kernel.elf

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

os.iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	genisoimage -R \
	-b boot/grub/stage2_eltorito \
	-no-emul-boot \
	-boot-load-size	4 \
	-A os \
	-input-charset	utf8 \
	-quiet \
	-boot-info-table \
	-o os.iso	\
	iso

run: os.iso
	qemu-system-x86_64 -kernel kernel.elf	
#bochs -f bochs.txt -q

%.o: %.s 
	$(AS) $(ASFLAGS) $< -o $@


%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o kernel.elf os.iso bochslog.txt com1.out
