ENTRY(loader)

SECTIONS {

	. = 0x00100000;	/* code loader after 1M (BIOS,GRUB,I/O ops before)*/

	.multiboot_header 0x00100000 : {
		KEEP(*(.multiboot_header))
	}	



	.boot ALIGN ( 4K) : {
		*(.boot)
	}
	

	/*  higher half */
	. += 0xC0000000;

	.text ALIGN (0x1000): AT (ADDR(.text) - 0xC0000000)

	{
		*(.text)
		*(.rodata)
	}

	.data ALIGN (0x1000): AT (ADDR(.data) - 0xC0000000)

	{
		*(.data)
	}	

	.bss ALIGN (0x1000): AT (ADDR(.bss) - 0xC0000000)

	{
		*(COMMON)
		*(.bss)
	}
		
}
