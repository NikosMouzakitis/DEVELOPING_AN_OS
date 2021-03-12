#include "io.c"
#include "framebuffer.h"
#include "serial.h"
#include "x86.h"

#define BG_GREEN	2
#define BG_BLACK	0 

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
	fb_writeln(osInitialmsg, strlen(osInitialmsg));
	serial_writeln(osInitialmsg, strlen(osInitialmsg),com);
	delay(100,1000,200);	
	serial_writeln("Developed by Nikos Mouzakitis", strlen("Developed by Nikos Mouzakitis"),com);
	fb_writeln("Developed by Nikos Mouzakitis", strlen("Developed by Nikos Mouzakitis"));
	delay(100,1000,200);	

	serial_writeln("2019-2020", strlen("2019-2020"), com);	
	fb_writeln("2019-2020", strlen("2019-2020"));
	
	//write first message on serial.
	serial_writeln("init", strlen("init"), com);	
	fb_writeln("init", strlen("init"));	

	serial_writeln("Setting Global Descriptor Table", strlen("Setting Global Descriptor Table"), com);	
	fb_writeln("Setting Global Descriptor Table", strlen("Setting Global Descriptor Table"));	
	//initializing the Global Descriptor Table

	init_gdt();
	serial_writeln("Global Descriptor Table done", strlen("Global Descriptor Table done"), com);	
	fb_writeln("Global Descriptor Table done", strlen("Global Descriptor Table done"));	
	while(1)
		;
}
