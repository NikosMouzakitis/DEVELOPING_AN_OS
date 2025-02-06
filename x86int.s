extern	isr_default_int, isr_timer_int, isr_kbd_int, do_syscalls, isr_schedule_int, isr_GP_exc, isr_PF_exc 

global	_asm_syscalls, _asm_exc_GP, _asm_exc_PF, _asm_schedule

global _asm_int_1
global _asm_int_2
global _asm_int_32
global _asm_int_33

%macro	SAVE_REGS 0
	pushad 
	push ds
	push es
	push fs
	push gs 
	push ebx
	mov bx,0x10
	mov ds,bx
	pop ebx
%endmacro

%macro	RESTORE_REGS 0
	pop gs
	pop fs
	pop es
	pop ds
	popad
%endmacro

%macro	INTERRUPT 1
global _asm_int_%1
_asm_int_%1:
	cli
	SAVE_REGS
	push %1
	call isr_default_int
	pop eax
	mov al,0x20
	out 0x20,al
	RESTORE_REGS
	sti ;re-enable irqs
	iret
%endmacro

INTERRUPT 1
INTERRUPT 2

; Special handling for timer interrupt (IRQ0 - int 0x20)
_asm_int_32:
	cli
	SAVE_REGS
	push 32
	call isr_timer_int  ; Call the actual timer ISR 
	pop eax
	mov al, 0x20
	out 0x20, al
	RESTORE_REGS
	sti ;reenable irqs
	iret

_asm_int_33:
	cli
	SAVE_REGS
	push 33
	call isr_kbd_int  ; Call the actual keyboard isr
	pop eax
	mov al, 0x20
	out 0x20, al
	RESTORE_REGS
	sti ;reenable irqs
	iret

