#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

void init_uart(unsigned long baud_rate){ 
	unsigned int ubrr = (F_CPU/16/baud_rate)-1;       
	UBRRH = (unsigned char)(ubrr>>8);        
	UBRRL = (unsigned char)ubrr;
	UCSRB = (1<<RXEN)|(1<<TXEN);
	UCSRC = (1<<URSEL) | (0<<6) | (0<<4) | (0<<3) |(3<<1) | (0<<0);
	
}

void usart_transmit_byte_val(uint8_t data){ 
	while (!(UCSRA & (1<<UDRE)));          
	UDR = data;
}


void usart_receive_byte_ref(uint8_t *data){ 
	while (!(UCSRA & (1<<RXC)));          
	*data = UDR;
}

int uart_putchar(char ch, FILE *stream){ 
	if(ch=='\n') uart_putchar('\r',stream); 
	usart_transmit_byte_val((uint8_t) ch); 
	return 0;
}


int uart_getchar(FILE *stream){ 
	char ch; usart_receive_byte_ref( (uint8_t *) &ch); 
	/* Echo the output back to the terminal */ 
	uart_putchar(ch,stream); 
	return ch;}

FILE uart_str = FDEV_SETUP_STREAM(uart_putchar, uart_getchar,_FDEV_SETUP_RW);

void stdio_serial_initialize(){ 
	stdout = stdin = &uart_str;
}

int main(void){ 
	init_uart(9600); 
	stdio_serial_initialize();  
	char name[20];    
	while(1){  
		printf("enter your name\r\n");  
		scanf("%s",name);  
		printf("your name is %s\r\n",name);      }
}


