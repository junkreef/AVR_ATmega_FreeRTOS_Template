//<one line to give the kernel_support.h name and a brief idea of what it does.>
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

#ifndef KERNEL_SUPPORT_H_
#define KERNEL_SUPPORT_H_

/*FreeRTOS Include Files*/
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>


void vApplicationIdleHook( void );
void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName );

#endif /* KERNEL_SUPPORT_H_ */
