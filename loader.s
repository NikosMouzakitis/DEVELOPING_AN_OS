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

section .bss
align 4
kernel_stack:
	resb	KERNEL_STACK_SIZE	; reserving the kernel stack.

section	.boot  
align	4
loader:	

	;paging
label1:
	mov ecx, (initial_page_dir - 0xC0000000)  
	mov cr3, ecx  ;where the pdt is 
	
	mov ecx, cr4
	or  ecx,0x10  ; required change in cr4
	mov cr4, ecx

	mov ecx, cr0
	or  ecx, 0x80000000 
	mov cr0, ecx  ;enable paging by setting cr0 register.
	jmp higher_half

;after here we are in higher half.
section .text
higher_half:
	mov	esp, kernel_stack+KERNEL_STACK_SIZE	; saving at esp to the start of the kernel start(grows downwards)
	push	ebx		; for getting the boot info as argument in kmain 
	push    eax		; magic number.
	xor     ebp, ebp
	call	kmain		; calling the C function from kmain.c, entering the OS
loop:
	jmp loop

section .data
align 4096
global initial_page_dir
;initial page directory.
initial_page_dir:
	dd 10000011b
	times 768-1  dd 0
	dd (0 << 22) | 10000011b
	dd (1 << 22) | 10000011b
	dd (2 << 22) | 10000011b
	dd (3 << 22) | 10000011b
	times 256-1  dd 0
