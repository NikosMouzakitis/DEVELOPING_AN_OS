#include "strlib.h"	//character array functionality

#define IDTBASE		0x00000000
#define IDTSIZE		0xff
#define INTGATE		0x8e00 //interrupts
#define TRAPGATE	0xef00 //traps
#define GDTSIZE		0xff
#define GDTBASE		0x00000800

typedef unsigned int	u32;
typedef unsigned short	u16;
typedef unsigned char	u8;

		/* Interrupt Descriptor Table */
// IDT description structure.
struct idtr {
	unsigned short limit;
	unsigned int base;
} __attribute__((packed));

// Structure of IDT segments.
struct idtdesc {
	unsigned short offset0_15;
	unsigned short select;
	unsigned short type;
	unsigned short offset16_31;
} __attribute__((packed));
struct idtr	kidtr;
struct idtdesc	kidt[IDTSIZE];	//IDT table.


//GDT entries
struct gdtr {
    u16 limit;
    u32 base;
} __attribute__ ((packed));

struct gdtdesc {
    u16 lim0_15;
    u16 base0_15;
    u8 base16_23;
    u8 acces;
    u8 lim16_19:4;
    u8 other:4;
    u8 base24_31;
} __attribute__ ((packed));


void init_idt(void);
void init_idt_desc(unsigned short select, unsigned int offset, unsigned short type,struct idtdesc *desc);
void init_gdt_desc(u32 base, u32 limit, u8 access, u8 other,struct gdtdesc *desc);
void init_gdt(void);
void do_syscalls(int num);
