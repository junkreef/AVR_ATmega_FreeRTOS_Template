//<one line to give the uart.c name and a brief idea of what it does.>
// Copyright (C) 2012 Junpei Kishi
//
//
//This program is free software; you can redistribute it and/or modify it under the terms of
// the GNU General Public License as published by the Free Software Foundation; either version 3 of the License,
// or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License along with this program.
// If not, see <http://www.gnu.org/licenses/>.

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "uart.h"


typedef struct
{
    char buf[tx_bufsize];  /*送信バッファサイズ*/
    uint8_t buf_in;     /* バッファに文字を押し込むときの位置*/
    uint8_t buf_size;   /*バッファに貯められている文字数 */
} TxBuf_t;
typedef struct
{
    char buf[rx_bufsize];
    uint8_t buf_in;     /* バッファに文字を押し込むときの位置*/
    uint8_t buf_size;   /*バッファに貯められている文字数 */
} RxBuf_t;

volatile TxBuf_t TxBuf;
volatile RxBuf_t RxBuf;

void uart_putchar(char c)
{
    while (TxBuf.buf_size > (tx_bufsize-1)) ;  /* バッファが空くまで待つ */
    cli(); //バッファを書き換える（読み出しも含む)操作は必ず割り込み禁止で!
    TxBuf.buf[TxBuf.buf_in++] = c;
    TxBuf.buf_size++;
    if (TxBuf.buf_in==tx_bufsize) TxBuf.buf_in=0;
    UCSR0B = UCSR0B | _BV(UDRIE0); /* UDR空き割り込みを有効にする。 */
    sei();
}

void uart_putstr(char *s)
{
    char c;
    for(;;)
    {
        c = *s++;
        if (c==0) break;
        uart_putchar(c);
    }
}

void uart_putstr_p(const prog_char *pgm_s)
{
	char c;
	c=pgm_read_byte(pgm_s);
	while ((c=pgm_read_byte(pgm_s++)) != 0)
		uart_putchar(c);
}

void uart_putbin(uint8_t data){
	if(data/128){ uart_putchar('1'); data = data-128;}
	else uart_putchar('0');

	if(data/64) { uart_putchar('1'); data = data-64;}
		else uart_putchar('0');

	if(data/32){ uart_putchar('1'); data = data-32;}
		else uart_putchar('0');

	if(data/16) { uart_putchar('1'); data = data-16;}
		else uart_putchar('0');

	if(data/8) { uart_putchar('1'); data = data-8;}
		else uart_putchar('0');

	if(data/4) { uart_putchar('1'); data = data-4;}
		else uart_putchar('0');

	if(data/2) { uart_putchar('1'); data = data-2;}
		else uart_putchar('0');

	if(data/1) { uart_putchar('1'); data = data-1;}
		else uart_putchar('0');
}

void uart_puthex(uint8_t byte)
{
	uint8_t i;
	uart_putchar('0');
	uart_putchar('x');
	i=(byte>>4);
	if(i<10)
		uart_putchar(i+'0');
	else
		uart_putchar(i+'a'-10);

	i=(byte&0x0f);
	if(i<10)
		uart_putchar(i+'0');
	else
		uart_putchar(i+'a'-10);

}

void uart_puthex2(uint16_t word)
{
	uint8_t i;
	uart_putchar('0');
	uart_putchar('x');
	i=((word>>12)&0x0f);
	if(i<10)
		uart_putchar(i+'0');
	else
		uart_putchar(i+'a'-10);
	i=((word>>8)&0x0f);
	if(i<10)
		uart_putchar(i+'0');
	else
		uart_putchar(i+'a'-10);
	i=((word>>4)&0x0f);
	if(i<10)
		uart_putchar(i+'0');
	else
		uart_putchar(i+'a'-10);
	i=((word)&0x0f);
	if(i<10)
		uart_putchar(i+'0');
	else
		uart_putchar(i+'a'-10);
}

void uart_putdec(uint16_t data){
	uart_putchar(data/1000 + '0');
	data %= 1000;
	uart_putchar(data/100 + '0');
	data %= 100;
	uart_putchar(data/10 + '0');
	data %= 10;
	uart_putchar(data + '0');
}

uint8_t uart_getch(void)
{
    char c;
    uint8_t buf_out;
    while (!RxBuf.buf_size) ;
    cli();
    buf_out=RxBuf.buf_in-RxBuf.buf_size;
    if (RxBuf.buf_in<RxBuf.buf_size) {
        buf_out += rx_bufsize;
    }
    c = RxBuf.buf[buf_out];
    if (RxBuf.buf_size==0) {
        RxBuf.buf_size += rx_bufsize;
    }
    RxBuf.buf_size--;
    sei();
    return c;
}

uint8_t uart_rcv_size(void){
return RxBuf.buf_size;
}

void uart_init(uint32_t baud){
	UCSR0A = 0b00000000;
	UCSR0B = _BV(TXEN0)|_BV(RXEN0) |_BV(RXCIE0);
	UBRR0=UBRR_Value(baud);
	RxBuf.buf_in = RxBuf.buf_size = 0;
	TxBuf.buf_size = 0;    /*バッファを空にする */
}

ISR(USART0_UDRE_vect){
    /* TxBuf.buf_size==0の状態でDATA割り込みを許可すると変なことになります。注意。 */
    uint8_t buf_out;
    buf_out=TxBuf.buf_in-TxBuf.buf_size;
    if (TxBuf.buf_in<TxBuf.buf_size) buf_out += tx_bufsize;
    UDR0 = TxBuf.buf[buf_out];
    TxBuf.buf_size--;
    if (TxBuf.buf_size==0) UCSR0B = UCSR0B & ~_BV(UDRIE0); /* UDR空き割り込み禁止 */
}

ISR(USART0_RX_vect){
     uint8_t c;
     c = UDR0;
     if (RxBuf.buf_size>=rx_bufsize) {
         return;
         //フロー制御なし
     }
     RxBuf.buf[RxBuf.buf_in++]=c;
     if (RxBuf.buf_in>=rx_bufsize) RxBuf.buf_in=0;
     RxBuf.buf_size++;
}
