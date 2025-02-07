#ifndef	__INCLUDE_FRAMEBUFFER_H
#define __INCLUDE_FRAMEBUFFER_H

#define SCREEN_WIDTH	80	
#define SCREEN_HEIGHT	25	
#define CELL_SIZE	2
extern int cursorPos;

//writes at framebuffer's cell (i) the character (c). 
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);

//framebuffer write function.(using fb_write_cell.
void fb_write(char *buf, unsigned int len);
void fb_writeln(char *buf, unsigned int len);

/* update position according to cursorPos */
void update_cursor_position(void);

/* advance in the next line	*/
void update_cursor_next_line(void);

/* move all lines up by 1 line */
void fb_scroll(void);

#endif
