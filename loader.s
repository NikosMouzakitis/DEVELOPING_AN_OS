global	loader				;entry symbol for ELF

extern	kmain	

MAGIC_NUMBER	equ 0x1BADB002		;magic number constant
FLAGS	equ	0x0			;flags for multiboot
CHECKSUM	equ -MAGIC_NUMBER	;calculating a checksum
KERNEL_STACK_SIZE	equ	4096	;size of stack in bytes.

section	.text:
align	4
	dd	MAGIC_NUMBER
	dd	FLAGS
	dd	CHECKSUM


section .bss:
align	4
kernel_stack:
	resb	KERNEL_STACK_SIZE	; reserving the kernel stack.

loader:	
	mov	eax, 0xCAFEBABE

	
	mov	esp, kernel_stack+KERNEL_STACK_SIZE	; saving at esp to the start of the kernel start(grows downwards)

	call	kmain		; calling the C function from kmain.c, entering the OS

loop:
	jmp	loop
