#include "framebuffer.h"
#include "io.h"

#define BG_GREEN	2
#define BG_BLACK	0
#define FBUF_ADR	0x000B8000

int cursorPos ; //cursor possition on the screen.(extern in .h)

void update_cursor_position(void)
{
	fb_move_cursor(cursorPos);
}

/* adnvancing cursor to the start of the next line of the framebuffer. */
void update_cursor_next_line(void)
{
	unsigned int ret;
	ret = cursorPos / 80;
	cursorPos = (ret+1)*80;
	fb_move_cursor( (ret+1)*80 );		
}

/* function called from fb_write */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
	char *fb = (char *) FBUF_ADR;

	fb[i] = c;
	fb[i+1] = ( (fg & 0x0f) << 4 ) | (bg & 0x0f);
}

/* writing on the framebuffer using memory mapped I/O. */
void fb_write(char *buf, unsigned int len)
{
	unsigned int i;
	
	for( i = 0; i < len; i++) {
		fb_write_cell(i*2, buf[i],BG_BLACK, BG_GREEN);
		cursorPos++;
	}
	update_cursor_position();
}

void fb_writeln(char *buf, unsigned int len)
{
	unsigned int i;
	
	for( i = 0; i < len; i++) {
		//fb_write_cell(i*2, buf[i],BG_BLACK, BG_GREEN);
		fb_write_cell(cursorPos*2, buf[i],BG_BLACK, BG_GREEN);
		cursorPos++;
	}
	update_cursor_next_line();
}
