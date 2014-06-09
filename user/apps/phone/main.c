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
#include <platform/stm32f4/ltdc.h>
#include <platform/stm32f4/fonts.h>
#define STACK_SIZE 256

struct button {
	uint16_t x,y,height,width;
};

static __USER_DATA struct button btn_list[14];
static __USER_DATA uint8_t index = 0;

uint8_t __USER_TEXT create_button(uint16_t x, uint16_t y, uint16_t height, uint16_t width, uint8_t c)
{
	lcd_set_text_color(0xCE79);
	lcd_fill_rect(x, y, height, width);
	lcd_set_back_color(0xCE79);
	lcd_set_text_color(LCD_COLOR_BLACK);
	lcd_draw_rect(x, y, height, width);
	lcd_print_char((x+(width)/2-8), (y+(height/2)-6), c);

	btn_list[index].x = x;
	btn_list[index].y = y;
	btn_list[index].height = height;
	btn_list[index].width = width;
	return (index++);
}

static void __USER_TEXT main(user_struct *user)
{
	uint8_t btn_one, btn_two, btn_three, btn_four, btn_five, btn_six, btn_seven,btn_eight, btn_nine, btn_zero, btn_hash, btn_asterik;

	uint32_t xpos, ypos, btn_height, btn_width;
    gpio_config_output(GPIOG, 13, GPIO_PUPDR_UP, GPIO_OSPEEDR_50M);
	lcd_init();
	lcd_layer_init();
	ltdc_layer_cmd(LTDC_Layer1, 1);
	ltdc_layer_cmd(LTDC_Layer2, 1);
	ltdc_reload_cfg(LTDC_IMReload);
	ltdc_cmd(1);
	lcd_set_layer(LCD_FOREGROUND_LAYER);
	lcd_clear(LCD_COLOR_WHITE);
	lcd_set_font(&Font12x12);

	// One button
	xpos = 5;
	ypos = LCD_PIXEL_HEIGHT / 2 - 10;
	btn_height = (LCD_PIXEL_HEIGHT / 2) / 4;
	btn_width = (LCD_PIXEL_WIDTH - 10) / 3;
	btn_one = create_button(xpos, ypos, btn_height, btn_width, '1');

	// Two button
	xpos += btn_width;
	btn_two = create_button(xpos, ypos, btn_height, btn_width, '2');

	// Three button
	xpos += btn_width;
	btn_three = create_button(xpos, ypos, btn_height, btn_width, '3');

	// Four button
	xpos = 5;
	ypos += btn_height;
	btn_four = create_button(xpos, ypos, btn_height, btn_width, '4');

	// Five button
	xpos += btn_width;
	btn_five = create_button(xpos, ypos, btn_height, btn_width, '5');

	// Six button
	xpos += btn_width;
	btn_six = create_button(xpos, ypos, btn_height, btn_width, '6');

	// Seven button
	xpos = 5;
	ypos += btn_height;
	btn_seven = create_button(xpos, ypos, btn_height, btn_width, '7');

	// Eight button
	xpos += btn_width;
	btn_eight = create_button(xpos, ypos, btn_height, btn_width, '8');

	// Nine button
	xpos += btn_width;
	btn_six = create_button(xpos, ypos, btn_height, btn_width, '9');

	// Asterik button
	xpos = 5;
	ypos += btn_height;
	btn_asterik = create_button(xpos, ypos, btn_height, btn_width, '*');

	// Zero button
	xpos += btn_width;
	btn_zero = create_button(xpos, ypos, btn_height, btn_width, '0');

	// Hash button
	xpos += btn_width;
	btn_hash = create_button(xpos, ypos, btn_height, btn_width, '#');
	
	btn_one = btn_two = btn_three = btn_four = btn_five = btn_six = btn_seven = btn_eight = btn_nine = btn_asterik = btn_zero = btn_hash; // To cancel warning
	btn_two = btn_one;

 	gpio_out_high(GPIOG, 13);
    while(1) {
        L4_Sleep(L4_Never);
    }
}

DECLARE_USER(
	5,
	phoneui,
	main,
	DECLARE_FPAGE(0x0, UTCB_SIZE + STACK_SIZE)
	DECLARE_FPAGE(0x40015000, 0x0c00)
	DECLARE_FPAGE(0x40020000, 0x3c00)
	DECLARE_FPAGE(0x40028000, 0x8000)
	DECLARE_FPAGE(0x42470000, 0x0c00)
	DECLARE_FPAGE(0x40016800, 0x0c00)
	DECLARE_FPAGE(0xA0000000, 0x1000)
	DECLARE_FPAGE(0xD0000000, 0xA0000)
	DECLARE_FPAGE(0xD00A0000, 0xA0000)
);
