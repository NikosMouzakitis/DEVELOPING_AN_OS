#include "io.c"
#include "framebuffer.h"
#include "serial.h"
#include "x86.h"

#define BG_GREEN	2
#define BG_BLACK	0 

// @author N.M
//
/* cleaning screen by using the framebuffer */
void clear_screen(void)
{
	int i;

	for (i = 0; i < 25*80*2; i+=2) {
		fb_write_cell(i,' ', BG_BLACK, BG_GREEN);
	}
	fb_move_cursor(0);	
}

//initialize com1
void set_up_com(unsigned short com) 
{
	serial_configure_baud_rate(com, 1);
	serial_configure_FIFObuffers(com);
	serial_configure_line(com);
	configure_modem(com);	
}

// introducing a small delay in the processor.
void delay(int i, int j, int k)
{
	int x, y, z;
	for( x = 0; x < i; x++)
		for(y = 0; y < j; y++)
			for(z = 0; z < k/10; z++)
				;
}

void print(char * s, int l, int com)
{
	fb_writeln(s, l);
	serial_writeln(s, l, SERIAL_COM1_BASE);
}
//wrapper to get an easier way to print-non formated!!!
void pprint(char *s)
{
	print(s,strlen(s),SERIAL_COM1_BASE);
}

/* start execution of testOS */
void kmain(void)
{
	char * osInitialmsg = "TestOS initialized.";
	int com = SERIAL_COM1_BASE;
//	struct gdtr GDT[8]; // exist in x86
	cursorPos = 0;
	//setting up COM1 and clearing screen.	
	set_up_com(com);	
	clear_screen();
	//display first message on screen.
	pprint(osInitialmsg);
	delay(100,1000,200);	
	//write first message on serial.
	pprint("init");	

	pprint("Setting Global Descriptor Table");	
	//initializing the Global Descriptor Table
	init_gdt();
	pprint("okay");	
	
	pprint("Initializing Interrupt Descriptor Table");	
	init_idt();	
	pprint("okay");

	pprint("Initializing PIC");
	init_pic();
	pprint("okay");

	pprint("Initializing PIT");
	init_pit(1);
	pprint("okay");

	pprint("Enabling irq's");
	pprint("okay");

	while(1)
	{
		__asm__ ("sti");
	//	asm volatile("hlt");
	//	asm volatile("int $0x20");  // Manually trigger the timer interrupt
	}


}
