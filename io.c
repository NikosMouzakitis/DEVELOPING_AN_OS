#include "io.h"

/* I/O ports */
#define FB_COMMAND_PORT		0x3D4
#define FB_DATA_PORT		0x3D5

/* I/O port commands */
#define	FB_HIGH_BYTE_COMMAND	14
#define	FB_LOW_BYTE_COMMAND	15



void fb_move_cursor(unsigned short pos)
{
	outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
	outb(FB_DATA_PORT, ( (pos >> 8) & 0x00ff) );	
	outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
	outb(FB_DATA_PORT, pos & 0x00ff );	
}

