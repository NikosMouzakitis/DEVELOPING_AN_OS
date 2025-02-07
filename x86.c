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
extern void _asm_int_32(void); //timer interrupt(pit)
extern void _asm_int_33(void); //keaboard interrupt(pit)

volatile int tick_count =0;

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
    int i;

    // Initialize all IDT entries with a default handler
    for (i = 0; i < IDTSIZE; i++) {
  //      init_idt_desc(0x08, (u32)isr_timer_int, INTGATE, &kidt[i]);
        init_idt_desc(0x08, (u32)_asm_int_1, INTGATE, &kidt[i]);
    }

    // Set up exceptions
   // init_idt_desc(0x08, (u32)_asm_exc_GP, INTGATE, &kidt[13]);  // #GP
   // init_idt_desc(0x08, (u32)_asm_exc_PF, INTGATE, &kidt[14]);  // #PF

    // Set up IRQ handlers
    init_idt_desc(0x08, (u32)_asm_int_32, INTGATE, &kidt[32]); //timer
    init_idt_desc(0x08, (u32)_asm_int_33, INTGATE, &kidt[33]); //keaboard

    // System call handling
  //   init_idt_desc(0x08, (u32)_asm_syscalls, TRAPGATE, &kidt[48]);  // Syscall
  //  init_idt_desc(0x08, (u32)_asm_syscalls, TRAPGATE, &kidt[128]); // Syscall

    // Set IDTR values
    kidtr.limit = sizeof(kidt) - 1;
    kidtr.base = (u32)kidt;

    // Load IDT
    asm volatile("lidt %0" : : "m" (kidtr));
    asm volatile("sti"); //set the interrupt flag.
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
    init_gdt_desc(0x0, 0x0, 0x0, 0x0, &kgdt[0]);  /* null descriptor */
    init_gdt_desc(0x0, 0xFFFFF, 0x9A, 0x0D, &kgdt[1]);  /* kernel code */
    init_gdt_desc(0x0, 0xFFFFF, 0x92, 0x0D, &kgdt[2]);  /* kernel data */
    init_gdt_desc(0x0, 0x0, 0x96, 0x0D, &kgdt[3]);      /* kernel stack */
    init_gdt_desc(0x0, 0xFFFFF, 0xFA, 0x0D, &kgdt[4]);  /* user code */
    init_gdt_desc(0x0, 0xFFFFF, 0xF2, 0x0D, &kgdt[5]);  /* user data */
    init_gdt_desc(0x0, 0x0, 0xF6, 0x0D, &kgdt[6]);      /* user stack */

    /* initialize the gdtr structure */
    kgdtr.limit = sizeof(kgdt) - 1;
    kgdtr.base = (u32) &kgdt;

    /* load the gdtr registry */
    asm("lgdtl (kgdtr)");

    /* reload segment registers */
    asm("   ljmp $0x08, $flush   \n\
          flush:                 \n\
          movw $0x10, %ax         \n\
          movw %ax, %ds           \n\
          movw %ax, %es           \n\
          movw %ax, %fs           \n\
          movw %ax, %gs           \n\
    ");
}

void isr_kbd_int(void)
{
	u8 scancode;
	//pprint("a");
	scancode = inb(0x60);  // Read scancode from the keyboard data port
	
	
	if(scancode == ENTER_KEY) {
		keyboard_buffer[kb_buffer_index]='\0'; //null terminated string.
		kb_buffer_index = 0; //reset
	//	process_command(keyboard_buffer); //call function to process user's command.
		fb_writeln("\n",1);		
	} else if (scancode == BACKSPACE_KEY) {
		if(kb_buffer_index > 0)
		{
			kb_buffer_index--; //decrement index.
			fb_write("\b \b",3); //erase visually one character.
		}
	} else {

		char c = scancode_to_ascii(scancode); //convertion
		if(c != '\0') //ensure it is not a release press action
		{
			if(c && kb_buffer_index < KEYBOARD_BUFFER_SIZE - 1) {
				keyboard_buffer[kb_buffer_index++] = c;
				fb_write(&c,1);
			}
		}
	}

	//acknowledge the interrupt.
	outb(0x20,0x20); //eoi
	outb(0xA0, 0x20);  // Send EOI to PIC2 (Slave PIC)
}

void isr_timer_int(void) {
	tick_count++;
//	pprint("tmr irq fired");
	outb(0x20, 0x20);  // Send EOI end of interrupt to PIC1 (IRQ0)
	outb(0xA0, 0x20);  // Send EOI to PIC2 (Slave PIC)
}

void isr_default_int(int num)
{
	return ;
}
/*
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
*/

// Programmable Interrupt Controller Initialization
void init_pic() {
    // Start initialization sequence (ICW1)
    outb(PIC1_COMMAND, 0x11); // Initialize PIC1
    outb(PIC2_COMMAND, 0x11); // Initialize PIC2

    // Remap interrupt vectors
    outb(PIC1_DATA, 0x20); // IRQ0-7 → INT 0x20-0x27
    outb(PIC2_DATA, 0x28); // IRQ8-15 → INT 0x28-0x2F

    // Set up cascading: PIC1 (IRQ2) connects to PIC2
    outb(PIC1_DATA, 4); // Tell PIC1 that PIC2 is on IRQ2 (0b00000100)
    outb(PIC2_DATA, 2); // Tell PIC2 that it is connected to PIC1 (IRQ2)

    // Enable 8086 mode
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);

    // Mask all interrupts except the timer (IRQ0)
  //  outb(PIC1_DATA, 0xFE); // 0xFD = 1111 1101 (Unmask IRQ0 and IRQ2)
    outb(PIC1_DATA, 0xFC); // 0xFD = 1111 1101 (Unmask IRQ0 and IRQ2)
    outb(PIC2_DATA, 0xFF); // Mask all interrupts on PIC2

    // Send End-of-Interrupt (EOI) to clear any pending interrupts
    outb(PIC1_COMMAND, 0x20);
    outb(PIC2_COMMAND, 0x20);
}

void init_pit(u32 frequency)
{
    u16 divisor = 1193180 / frequency; // PIT frequency is 1.19318 MHz
    outb(PIT_COMMAND, 0x36);
    outb(PIT_CHANNEL0, divisor & 0xFF);
    outb(PIT_CHANNEL0, (divisor >> 8) & 0xFF);

}
