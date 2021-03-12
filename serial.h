#include "io.h"

/* I/O ports */
#define SERIAL_COM1_BASE	0x3F8
#define SERIAL_DATA_PORT(base)	(base)
#define SERIAL_FIFO_COMMAND_PORT(base)	(base+2)
#define SERIAL_LINE_COMMAND_PORT(base)	(base+3)
#define SERIAL_MODEM_COMMAND_PORT(base)	(base+4)
#define SERIAL_LINE_STATUS_PORT(base)	(base+5)
#define SERIAL_LINE_ENABLE_DLAB	0x80

void serial_configure_baud_rate(unsigned short com, unsigned short divisor);
void serial_configure_FIFObuffers(unsigned short com);
void serial_configure_line(unsigned short com);
void configure_modem(unsigned short com);
int serial_is_transmit_fifo_empty(unsigned int com);
void serial_write(char *s, unsigned int len, unsigned short com);
void serial_writeln(char *s, unsigned int len, unsigned short com);
