//<one line to give the uart.h name and a brief idea of what it does.>
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


#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <avr/pgmspace.h>

#define rx_bufsize (25)
#define tx_bufsize (25)

#define UBRR_Value(b)       (uint16_t)((uint32_t)F_CPU/(uint64_t)(16*b)-1)
#define UBRR_U2XValue(b)    ((unsigned long)F_CPU-4*(unsigned long)b)/( 8*(unsigned long)b)-1

void uart_putchar(char c);
void uart_putstr(char *s);
void uart_putstr_p(const prog_char *pgm_s);
uint8_t uart_getch(void);
uint8_t uart_rcv_size(void);
void uart_init(uint32_t baud);
void uart_puthex(uint8_t byte);
void uart_putdec(uint16_t data);
void uart_putbin(uint8_t data);


#endif /* UART_H_ */
