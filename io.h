#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

//buffer size for populated codes from keyboard interrupts.
#define KEYBOARD_BUFFER_SIZE 256
//used in the keyboard scancodes
#define ENTER_KEY 0x1C
#define BACKSPACE_KEY 0x0E
#define KEY_RELEASE_MASK 0x80 // Mask for the release bit

extern char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
extern int kb_buffer_index; //index used for keyboard_buffer.


/* wrappers of assembly code in [io.s] */
void outb(unsigned short port, unsigned char data);
unsigned char inb(unsigned short port);


/*prototypes*/
void fb_move_cursor(unsigned short pos);

#endif
