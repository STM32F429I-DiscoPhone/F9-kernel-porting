/* Copyright (c) 2013 The F9 Microkernel Project. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include <platform/link.h>
#include <user_runtime.h>
#include <l4/utcb.h>
#include <l4/ipc.h>
#include <l4io.h>
#include <platform/stm32f4/gpio.h>
#include <platform/stm32f4/usart.h>

#define STACK_SIZE 256

static void __USER_TEXT main(user_struct *user)
{
    struct usart_dev console_uart;
    console_uart.u_num = 4;
    console_uart.baud = 115200;
    console_uart.base = USART2_BASE;
    console_uart.rcc_apbenr = RCC_USART2_APBENR;
    console_uart.rcc_reset = RCC_APB1RSTR_USART2RST;
    console_uart.tx.port = GPIOA;
    console_uart.tx.pin = 2;
    console_uart.tx.pupd = GPIO_PUPDR_NONE;
    console_uart.tx.type = GPIO_MODER_ALT;
    console_uart.tx.func = af_usart2;
    console_uart.tx.o_type = GPIO_OTYPER_PP;
    console_uart.tx.speed = GPIO_OSPEEDR_50M;
    console_uart.rx.port = GPIOA;
    console_uart.rx.pin = 3;
    console_uart.rx.pupd = GPIO_PUPDR_NONE;
    console_uart.rx.type = GPIO_MODER_ALT;
    console_uart.rx.func = af_usart2;
    console_uart.rx.o_type = GPIO_OTYPER_PP;
    console_uart.rx.speed = GPIO_OSPEEDR_50M;    
    usart_init(&console_uart);

    while(1) {
        usart_putc(&console_uart, 'a');
	while (!usart_status(&console_uart, USART_TC))
		/* wait */ ;
        printf("test\n");
        L4_Sleep(L4_TimePeriod(1000 * 100));
    }
}

DECLARE_USER(
	0,
	gpioer,
	main,
	DECLARE_FPAGE(0x0, UTCB_SIZE + STACK_SIZE)
	DECLARE_FPAGE(0x40004000, 0x1000)
	DECLARE_FPAGE(0x40020000, 0x3c00)
);
