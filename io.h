#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

/* wrappers of assembly code in [io.s] */
void outb(unsigned short port, unsigned char data);
unsigned char inb(unsigned short port);


/*prototypes*/
void fb_move_cursor(unsigned short pos);

#endif
