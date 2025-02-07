#include "framebuffer.h"
#include "io.h"

#define BG_GREEN	2
#define BG_BLACK	0
#define FBUF_ADR	0x000B8000

int cursorPos ; //cursor possition on the screen.(extern in .h)

void fb_scroll(void) {
    char *fb = (char *) FBUF_ADR;

    // Move every line up
    for (int row = 1; row < SCREEN_HEIGHT; row++) {
        for (int col = 0; col < SCREEN_WIDTH; col++) {
            int from_index = (row * SCREEN_WIDTH + col) * 2;
            int to_index   = ((row - 1) * SCREEN_WIDTH + col) * 2;
            fb[to_index] = fb[from_index];
            fb[to_index + 1] = fb[from_index + 1];
        }
    }

    // Clear last row
    for (int col = 0; col < SCREEN_WIDTH; col++) {
        int index = ((SCREEN_HEIGHT - 1) * SCREEN_WIDTH + col) * 2;
        fb[index] = ' ';
        fb[index + 1] = 0x07;  // White foreground, black background
    }

    // Fix cursor position
    cursorPos = (SCREEN_HEIGHT - 1) * SCREEN_WIDTH;
    fb_move_cursor(cursorPos);
}

void update_cursor_position(void)
{
	fb_move_cursor(cursorPos);
}

/* advancing cursor to the start of the next line of the framebuffer. */
void update_cursor_next_line(void)
{
	unsigned int ret = cursorPos/80;
	if(cursorPos >= (SCREEN_WIDTH*SCREEN_HEIGHT-SCREEN_WIDTH-1)) {
		//we are last line.
		fb_scroll();
	} else {
		cursorPos = (ret+1)*80;
		fb_move_cursor( (ret+1)*80 );
	}
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
		fb_write_cell(cursorPos*2, buf[i],BG_BLACK, BG_GREEN);
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

void fb_write_test_pattern() {
    char *fb = (char *) FBUF_ADR;
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT * 2; i += 2) {
        fb[i] = 'A' + (i / 2) % 26; // Write letters A-Z repeatedly
        fb[i + 1] = 0x0F;           // White foreground, black background
    }
}

