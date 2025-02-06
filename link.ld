ENTRY(loader)

SECTIONS {

	. = 0x00100000;	/* code loader after 1M (BIOS,GRUB,I/O ops before)*/

	.multiboot_header 0x00100000 : {
		KEEP(*(.multiboot_header))
	}	

	.text ALIGN (0x1000):
	{
		*(.text)
		*(.rodata)
	}

	.data ALIGN (0x1000):
	{
		*(.data)
	}	

	.bss ALIGN (0x1000):
	{
		*(COMMON)
		*(.bss)
	}
}
