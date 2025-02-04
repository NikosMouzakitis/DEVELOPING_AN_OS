#include "x86.h"
#include "io.h"

struct idtdesc 	kidt[IDTSIZE]; 		/* IDT table */
struct gdtdesc 	kgdt[GDTSIZE];		/* GDT */
struct gdtr 		kgdtr;				/* GDTR */
struct idtr 		kidtr; 				/* IDTR registry */
u32 *		stack_ptr=0;

extern void _asm_int_0(void);
extern void _asm_int_1(void);
extern void _asm_syscalls(void);
extern void _asm_exc_GP(void);
extern void _asm_exc_PF(void);
extern void _asm_schedule(void);

// Function to define an IDT segment
void init_idt_desc(unsigned short select, unsigned int offset, unsigned short type,struct idtdesc *desc)
{
	
	desc->offset0_15 = (offset & 0xffff);
	desc->select = select;
	desc->type = type;
	desc->offset16_31 = (offset & 0xffff0000) >> 16;
	
}

/*
 * Init IDT after kernel is loaded
 */

void init_idt(void)
{
	// Init irq 
	
	int i;
	for (i = 0; i < IDTSIZE; i++) 
		init_idt_desc(0x08, (u32)_asm_schedule, INTGATE, &kidt[i]); // 
	
	// Vectors  0 -> 31 are for exceptions 
	init_idt_desc(0x08, (u32) _asm_exc_GP, INTGATE, &kidt[13]);		// #GP 
	init_idt_desc(0x08, (u32) _asm_exc_PF, INTGATE, &kidt[14]);     // #PF 
	
	init_idt_desc(0x08, (u32) _asm_schedule, INTGATE, &kidt[32]);
	init_idt_desc(0x08, (u32) _asm_int_1, INTGATE, &kidt[33]);
	
	init_idt_desc(0x08, (u32) _asm_syscalls, TRAPGATE, &kidt[48]);
	init_idt_desc(0x08, (u32) _asm_syscalls, TRAPGATE, &kidt[128]); //48
	
	kidtr.limit = IDTSIZE * 8;
	kidtr.base = IDTBASE;
	
	
	// Copy the IDT to the memory 
	memcpy((char *) kidtr.base, (char *) kidt, kidtr.limit);

	// Load the IDTR registry
	asm("lidtl (kidtr)");
	
}

/*
 * 'init_desc' initialize a segment descriptor in gdt or ldt.
 * 'desc' is a pointer to the address
 */
void init_gdt_desc(u32 base, u32 limit, u8 access, u8 other,struct gdtdesc *desc)
{
	desc->lim0_15 = (limit & 0xffff);
	desc->base0_15 = (base & 0xffff);
	desc->base16_23 = (base & 0xff0000) >> 16;
	desc->acces = access;
	desc->lim16_19 = (limit & 0xf0000) >> 16;
	desc->other = (other & 0xf);
	desc->base24_31 = (base & 0xff000000) >> 24;
}

/*
 * This function initialize the GDT after the kernel is loaded.
 */
void init_gdt(void)
{
	/* initialize gdt segments */
	init_gdt_desc(0x0, 0x0, 0x0, 0x0, &kgdt[0]);
	init_gdt_desc(0x0, 0xFFFFF, 0x9B, 0x0D, &kgdt[1]);	/* code */
	init_gdt_desc(0x0, 0xFFFFF, 0x93, 0x0D, &kgdt[2]);	/* data */
	init_gdt_desc(0x0, 0x0, 0x97, 0x0D, &kgdt[3]);		/* stack */
	init_gdt_desc(0x0, 0xFFFFF, 0xFF, 0x0D, &kgdt[4]);	/* ucode */
	init_gdt_desc(0x0, 0xFFFFF, 0xF3, 0x0D, &kgdt[5]);	/* udata */
	init_gdt_desc(0x0, 0x0, 0xF7, 0x0D, &kgdt[6]);		/* ustack */
//	init_gdt_desc((u32)&default_tss, 0x67, 0xE9, 0x00, &kgdt[7]);	/* tss */

	/* initialize the gdtr structure */
	kgdtr.limit = GDTSIZE * 8;
	kgdtr.base = GDTBASE;

	/* copy the gdtr to its memory area */
	memcpy((char *) kgdtr.base, (char *) kgdt, kgdtr.limit);

	// AT&T style assembly
	/* load the gdtr registry */
	asm("lgdtl (kgdtr)");

	/* initiliaze the segments */
	asm("   movw $0x10, %ax	\n \
                movw %ax, %ds	\n \
                movw %ax, %es	\n \
                movw %ax, %fs	\n \
                movw %ax, %gs	\n \
                ljmp $0x08, $next	\n \
                next:		\n");
}

void isr_bbd_int(void)
{
	u8	i;
//	static int lshift_enable;
//	static int rshift_enable;
//	static int alt_enable;
//	static int ctrl_enable;
	
	do {
		i = inb(0x64);
	} while ( (i & 0x01) == 0);

}


// hide for the mo
void do_syscalls(int num){
	 u32 ret,ret1,ret2,ret3,ret4;
	 
	 asm("mov %%ebx, %0": "=m"(ret):);
	 asm("mov %%ecx, %0": "=m"(ret1):);
	 asm("mov %%edx, %0": "=m"(ret2):);
	 asm("mov %%edi, %0": "=m"(ret3):);
	 asm("mov %%esi, %0": "=m"(ret4):);
	  //fix the following comes from C++ code. 
	 //arch.setParam(ret,ret1,ret2,ret3,ret4);
	
	 asm("cli");
	 asm("mov %%ebp, %0": "=m"(stack_ptr):);

	 
	 //syscall.call(num);
	 
	 asm("sti");
}
void isr_default_int(int num)
{
	return ;
}
void isr_schedule_int(int num)
{
	return ;
}
void isr_GP_exc(int num)
{
	return ;
}
void isr_PF_exc(int num)
{
	return ;
}
