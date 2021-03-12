#include "serial.h"
/*	
		serial_configure_baud_rate:
  sets the speed of the data being sent. default at 115200bits/sec.
  Argument is divisor of this number.
  @param com		the COM port to configure
  @param divisor	the divisor	
 */
void serial_configure_baud_rate(unsigned short com, unsigned short divisor)
{
	outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);
	outb(SERIAL_DATA_PORT(com), (divisor >> 8) & 0x00FF);
	outb(SERIAL_DATA_PORT(com), divisor  & 0x00FF);
}

/*	
		serial_configure_FIFObuffers:
  Configures the FIFO queue. 0xc7 enables FIFO clears both receiver and transmission 
  FIFO queues and uses 14 bytes as the size of the queue.
*/
void serial_configure_FIFObuffers(unsigned short com)
{
	outb(SERIAL_FIFO_COMMAND_PORT(com), 0xC7);
}


/*	
		serial_configure_line:
  Configures the line of the given serial port.
  Explain a bit more the argument passes(0x03).
*/
void serial_configure_line(unsigned short com)
{
	outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

/*	
		serial_modem:
  set up with RTS DTR as 1 which means that we are ready to send data.  
*/
void configure_modem(unsigned short com)
{
	outb(SERIAL_MODEM_COMMAND_PORT(com), 0x03);
}

/*
		serial_is_transmit_fifo_empty:
 checks whether the transmit fifo is empty by calling inb routine, written
 in assembly in 'io.s' file.
*/
int serial_is_transmit_fifo_empty(unsigned int com)
{
	/*0x20* 0010 0000 */
	return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

void serial_write(char *s, unsigned int len, unsigned short com)
{
	unsigned int i = 0;

	while( i < len) {

		while( !serial_is_transmit_fifo_empty(com))
			;
		outb(SERIAL_DATA_PORT(com), s[i]);
		i++;
	}
}

void serial_writeln(char *s, unsigned int len, unsigned short com)
{
	unsigned int i = 0;

	while( i < len) {

		while( !serial_is_transmit_fifo_empty(com))
			;
		outb(SERIAL_DATA_PORT(com), s[i]);
		i++;
	}
//	question :: why it takes twice to do the carriage return?	
	outb(SERIAL_DATA_PORT(com) ,'\n');
	outb(SERIAL_DATA_PORT(com) ,'\n');
}
