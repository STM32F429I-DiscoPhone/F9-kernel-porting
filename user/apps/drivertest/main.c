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
#include <platform/stm32f4/rcc.h>
#include <platform/stm32f4/lcd.h>
#define STACK_SIZE 256

static void __USER_TEXT main(user_struct *user)
{
    int flag = 0;
	RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOAEN, 1);
	gpio_config_input(GPIOA, 0, GPIO_PUPDR_NONE);
    gpio_config_output(GPIOG, 13, GPIO_PUPDR_UP, GPIO_OSPEEDR_50M);
    gpio_config_output(GPIOG, 14, GPIO_PUPDR_UP, GPIO_OSPEEDR_50M);
	lcd_init();
    while(1) {
		flag = gpio_input_bit(GPIOA, 0);

        if(flag) {
            gpio_out_high(GPIOG, 13);
            gpio_out_low(GPIOG, 14);
        }
        else {
            gpio_out_high(GPIOG, 14);
            gpio_out_low(GPIOG, 13);
        }
		
        L4_Sleep(L4_TimePeriod(1000 * 100));
    }
}

DECLARE_USER(
	0,
	gpioer,
	main,
	DECLARE_FPAGE(0x0, UTCB_SIZE + STACK_SIZE)
//	DECLARE_FPAGE(0x40010000, 0x3c00)
	DECLARE_FPAGE(0x40015000, 0x0c00)
	DECLARE_FPAGE(0x40020000, 0x3c00)
	DECLARE_FPAGE(0x42470000, 0x0c00)
	DECLARE_FPAGE(0x40016800, 0x0c00)
	DECLARE_FPAGE(0xA0000000, 0x1000)
);
