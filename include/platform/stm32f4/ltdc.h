#ifndef PLATFORM_STM32F4_LTDC_H__
#define PLATFORM_STM32F4_LTDC_H__

#include <platform/link.h>
#include <platform/stm32f4/registers.h>

#define LTDC_HSPolarity_AL               ((uint32_t)0x00000000)                  /* Horizontal Synchronization is active low */
#define LTDC_HSPolarity_AH               ((uint32_t)0x80000000)                  /* Horizontal Synchronization is active high */

#define LTDC_VSPolarity_AL               ((uint32_t)0x00000000)                  /* Vertical Synchronization is active low */
#define LTDC_VSPolarity_AH               ((uint32_t)0x40000000)                  /* Vertical Synchronization is active high */

#define LTDC_DEPolarity_AL               ((uint32_t)0x00000000)                  /* Data Enable, is active low */
#define LTDC_DEPolarity_AH               ((uint32_t)0x20000000)                  /* Data Enable, is active high */

#define LTDC_PCPolarity_IPC              ((uint32_t)0x00000000)                  /* input pixel clock */
#define LTDC_PCPolarity_IIPC             ((uint32_t)0x10000000)                  /* inverted input pixel clock */


struct ltdc_cfg {
	uint32_t hs_polarity;
	uint32_t vs_polarity;
	uint32_t de_polarity;
	uint32_t pc_polarity;
	uint32_t horizontal_sync;
	uint32_t vertical_sync;
	uint32_t accumulated_hbp;
	uint32_t accumulated_vbp;
	uint32_t accumulated_active_w;
	uint32_t accumulated_active_h;
	uint32_t total_width;
	uint32_t total_height;
	uint32_t bg_red_value;
	uint32_t bg_green_value;
	uint32_t bg_blue_value;
};

void ltdc_init(struct ltdc_cfg *cfg);
#endif
