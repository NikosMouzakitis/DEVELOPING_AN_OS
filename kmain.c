#include "paging.h"
#include "io.c"
#include "framebuffer.h"
#include "serial.h"
#include "x86.h"
#include "multiboot.h"

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

//test an address if its mapped on pg_table
void test_access(u32 addr)
{
	char *e = (char *)addr;
	*e = 'a';
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
void kmain(u32 magic, struct multiboot_info *bootInfo)
{
	char * osInitialmsg = "TestOS initialized.";
	int com = SERIAL_COM1_BASE;
	cursorPos = 0; //reset.
	//setting up COM1 and clearing screen.	
	set_up_com(com); //abandoned com. // full prints on framebuffer. fix serial to follow all.	
	clear_screen();
//	fb_write_test_pattern(); //test.
//
	//display first message on screen.
	pprint(osInitialmsg);
	delay(100,1000,200);	
	//write first message on serial.

	pprint("Setting Global Descriptor Table");	
	//initializing the Global Descriptor Table
	init_gdt();
	
	pprint("Initializing Interrupt Descriptor Table");	
	init_idt();	

	pprint("Initializing PIC");
	init_pic();

	pprint("Initializing PIT");
	init_pit(1);

	//check GRUB passed values 

	//where physical memory starts? 
        u32 mod1 = *(u32 *)(bootInfo->mods_addr + 4);
	u32 physicalAllocStart = (mod1 + 0xFFF) & ~0xFFF;
	printf("mod1: %x",mod1);
	pprint("");
	printf("phAl: %x",physicalAllocStart);
	pprint("");

	printf("memH: %x",bootInfo->mem_upper*1024);
	pprint("");	
	pprint("Enabling irq's");
	pprint("okay");
	pprint(">>");

	//high point, low point, fetched values from GRUB multiboot information.
	init_memory(bootInfo->mem_upper * 1024, physicalAllocStart);	
	
	//Paging initialization
//	init_paging();

	while(1)
	{
		asm volatile("hlt");
	}

}
