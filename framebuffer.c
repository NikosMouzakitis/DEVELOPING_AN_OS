#include "framebuffer.h"
#include "io.h"

#define BG_GREEN	2
#define BG_BLACK	0
#define FBUF_ADR	0x000B8000

#define SERIAL_COM1_BASE       0x3F8

int cursorPos ; //cursor possition on the screen.(extern in .h)

//lookup table to convert keyboard scan codes to ASCII.
const char scancode_ascii[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' ', 0
};

// Helper function to print a single character
void putchar(char c) {
    fb_write_cell(cursorPos*2,c,BG_BLACK,BG_GREEN);        // Write to framebuffer
    cursorPos++;
    update_cursor_position();
    //serial_write(SERIAL_COM1_BASE, c);  // Write to serial port
}

// Helper function to print a string
void print_string(const char* str) {
    while (*str) {
        putchar(*str++);
    }
}

// Helper function to print a hexadecimal number
void print_hex(unsigned int num) {
    char buffer[9];  // 8 hex digits + null terminator
    const char* hex_digits = "0123456789ABCDEF";
    int i = 7;

    buffer[8] = '\0';  // Null-terminate the string

    // Convert the number to hexadecimal
    do {
        buffer[i--] = hex_digits[num & 0xF];
        num >>= 4;
    } while (i >= 0);

    print_string(buffer);
}

// Helper function to print a signed integer
void print_int(int num) {
    char buffer[12];  // Enough for 32-bit integers
    int i = 0;

    if (num < 0) {
        putchar('-');
        num = -num;
    }

    // Convert the number to a string
    do {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    } while (num > 0);

    // Print the string in reverse order
    while (i > 0) {
        putchar(buffer[--i]);
    }
}

// printf-style function
void printf(const char* format, ...) {
    // Access the variable arguments directly from the stack
    unsigned int* arg_ptr = (unsigned int*)&format + 1;

    while (*format) {
        if (*format == '%') {
            format++;  // Move past '%'
            switch (*format) {
                case 'x':  // Hexadecimal
                    print_hex(*arg_ptr++);
                    break;
                case 's':  // String
                    print_string((const char*)*arg_ptr++);
                    break;
                case 'c':  // Character
                    putchar((char)*arg_ptr++);
                    break;
                case 'd':  // Signed decimal
                    print_int((int)*arg_ptr++);
                    break;
                case '%':  // Literal '%'
                    putchar('%');
                    break;
                default:   // Unsupported format specifier
                    putchar('%');
                    putchar(*format);
                    break;
            }
        } else {
            putchar(*format);
        }
        format++;
    }
}


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

char scancode_to_ascii(unsigned char scancode) {
	if(scancode & KEY_RELEASE_MASK)
		return '\0';

	return scancode_ascii[scancode];
}
