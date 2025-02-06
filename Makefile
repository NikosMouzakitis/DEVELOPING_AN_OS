#OBJECTS= loader.o io.o x86.o framebuffer.o serial.o strlib.o kmain.o
OBJECTS= loader.o io.o x86int.o x86.o framebuffer.o serial.o strlib.o kmain.o

CC= gcc

CFLAGS= -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
	-nostartfiles -nodefaultlibs -Wall -Wextra -Wno-error -Wno-unused-variable -Wunused-parameter \
        -c -g

LDFLAGS= -T link.ld -melf_i386

AS= nasm

ASFLAGS= -f  elf

all: kernel.elf

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

os.iso: kernel.elf
	# Copy the kernel into the ISO folder
	cp kernel.elf iso/boot/
	# Use grub-mkrescue to create the ISO image.
	# Note: On some systems you might need to specify --modules="linux" or additional options.
	grub-mkrescue -o os.iso iso	
	
	
run: os.iso
	bochs -f bochs.txt -q
	#qemu-system-x86_64 -kernel kernel.elf	

%.o: %.s 
	$(AS) $(ASFLAGS) $< -o $@


%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o kernel.elf os.iso bochslog.txt com1.out
