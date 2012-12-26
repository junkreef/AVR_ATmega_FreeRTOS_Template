//<one line to give the main.c name and a brief idea of what it does.>
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

/*FreeRTOS Include Files*/
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

/*Private Include Files*/
#include <avr/io.h>
#include "uart.h"

void prvTaskA(void *pvParameters);
void prvTaskB(void *pvParameters);
void prvTaskC(void *pvParameters);

int main(void){
	uart_init(19200);
	uart_putstr("\r\n\n\n\nFreeRTOS Test\r\n\n");
	xTaskCreate(prvTaskA, "TaskA", 128, NULL, 1, NULL);
	xTaskCreate(prvTaskB, "TaskB", 128, NULL, 1, NULL);
	xTaskCreate(prvTaskC, "TaskC", 128, NULL, 1, NULL);
	vTaskStartScheduler();
	while(1)
		;
	return 0;
}

void prvTaskA(void *pvParameters){
	while(1){
		uart_putstr("A");
	}
}

void prvTaskB(void *pvParameters){
	while(1){
		uart_putstr("B");
	}
}

void prvTaskC(void *pvParameters){
	while(1){
		uart_putstr("C");
	}
}
