global	loader				;entry symbol for ELF
extern	kmain	

align 4
MAGIC_NUMBER	equ 0x1BADB002		;magic number constant
FLAGS	equ	0x3			;flags for multiboot, can get info about various aspects from GRUB2.
CHECKSUM	equ -MAGIC_NUMBER-FLAGS	;calculating a checksum
KERNEL_STACK_SIZE	equ	4096	;size of stack in bytes.

section .multiboot_header
align	4
	dd	MAGIC_NUMBER
	dd	FLAGS
	dd	CHECKSUM

section	.text
align	4
loader:	
	mov	eax, 0xCAFEBABE
	mov	esp, kernel_stack+KERNEL_STACK_SIZE	; saving at esp to the start of the kernel start(grows downwards)
	call	kmain		; calling the C function from kmain.c, entering the OS
loop:
	jmp loop

section .bss
align 4
kernel_stack:
	resb	KERNEL_STACK_SIZE	; reserving the kernel stack.


