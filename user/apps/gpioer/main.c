/* Copyright (c) 2013 The F9 Microkernel Project. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include <platform/link.h>
#include <user_runtime.h>
#include <l4/utcb.h>
#include <l4/ipc.h>
#include <l4io.h>
#include <gpioer.h>

static L4_Word_t free_mem __USER_DATA;
static L4_Word_t last_thread __USER_DATA;

static inline void __USER_TEXT led_init(void)
{
    _gpio_config_output(GPIOG, 13, GPIO_PUPDR_UP, GPIO_OSPEEDR_50M);
    _gpio_config_output(GPIOG, 14, GPIO_PUPDR_UP, GPIO_OSPEEDR_50M);
}

static inline void __USER_TEXT led_on(void)
{
    _gpio_out_high(GPIOG, 13);
    _gpio_out_high(GPIOG, 14);
}

void __USER_TEXT gpioer_thread(void)
{
    led_init();
    led_on();

    while(1);
}

static void __USER_TEXT start_thread(L4_ThreadId_t t, L4_Word_t ip,
                                     L4_Word_t sp, L4_Word_t stack_size)
{
	L4_Msg_t msg;

	L4_MsgClear(&msg);
	L4_MsgAppendWord(&msg, ip);
	L4_MsgAppendWord(&msg, sp);
	L4_MsgAppendWord(&msg, stack_size);
	L4_MsgLoad(&msg);

	L4_Send(t);
}

#define STACK_SIZE 256

static L4_ThreadId_t __USER_TEXT create_thread(user_struct *user, void (*func)(void))
{
	L4_ThreadId_t myself = L4_MyGlobalId();
	L4_ThreadId_t child;

	child.raw = myself.raw + (++last_thread << 14);

	L4_ThreadControl(child, myself, L4_nilthread, myself, (void *) free_mem);
	free_mem += UTCB_SIZE + STACK_SIZE;

	start_thread(child, (L4_Word_t)func, free_mem, STACK_SIZE);

	return child;
}

static void __USER_TEXT main(user_struct *user)
{
    L4_Sleep(L4_TimePeriod(1000 * 1000));
    printf("0x40020000: 0x%x\n", *(unsigned int *)0x40020000);
    L4_Sleep(L4_TimePeriod(1000 * 1000));
    printf("0x40021500: 0x%x\n", *(unsigned int *)0x40021500);

    free_mem = user->fpages[0].base;

    create_thread(user, gpioer_thread);
}

DECLARE_USER(
	0,
	gpioer,
	main,
	DECLARE_FPAGE(0x0, 2 * UTCB_SIZE + 2 * STACK_SIZE)
	DECLARE_FPAGE(0x40020000, 0x3c00)
);
