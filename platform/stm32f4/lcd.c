#include <platform/stm32f4/lcd.h>
#include <platform/stm32f4/gpio.h>
#include <platform/stm32f4/spi.h>
#include <platform/stm32f4/rcc.h>
#include <platform/stm32f4/sdram.h>
#include <platform/stm32f4/ltdc.h>

static __USER_DATA sFONT *lcd_current_font;

void __USER_TEXT lcd_init(void)
{
	struct ltdc_cfg ltdc_cfg;

	lcd_ctrllines_init();
	//reset & then set
	lcd_chipselect(0);
	lcd_chipselect(1);

	lcd_spi_config();

	lcd_poweron();

	RCC_APB2PeriphClockCmd(RCC_APB2ENR_LTDCEN, 1);

	RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_DMA2DEN, 1);

	lcd_af_gpio_init();

	sdram_init();

	ltdc_cfg.hs_polarity = LTDC_HSPolarity_AL;
	ltdc_cfg.vs_polarity = LTDC_VSPolarity_AL;
	ltdc_cfg.de_polarity = LTDC_DEPolarity_AL;
	ltdc_cfg.pc_polarity = LTDC_PCPolarity_IPC;

	ltdc_cfg.bg_red_value = 0;
	ltdc_cfg.bg_green_value = 0;
	ltdc_cfg.bg_blue_value = 0;

	RCC_PLLSAIConfig(192, 7, 4);
	RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div8);

	RCC_PLLSAICmd(1);

	while (RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY) == 0) ;

	ltdc_cfg.horizontal_sync = 9;
	ltdc_cfg.vertical_sync = 1;
	ltdc_cfg.accumulated_hbp = 29;
	ltdc_cfg.accumulated_vbp = 3;
	ltdc_cfg.accumulated_active_w = 269;
	ltdc_cfg.accumulated_active_h = 323;
	ltdc_cfg.total_width = 279;
	ltdc_cfg.total_height = 327;

	ltdc_init(&ltdc_cfg);
}

void __USER_TEXT lcd_write_cmd(uint8_t lcdreg)
{
	lcd_ctrllines_write(LCD_WRX_GPIO_PORT, LCD_WRX_PIN, 0);

	lcd_chipselect(0);
	spi_i2s_send(LCD_SPI, lcdreg);

	while (spi_i2s_get_flagstatus(LCD_SPI, SPI_I2S_FLAG_TXE) == 0);

	while (spi_i2s_get_flagstatus(LCD_SPI, SPI_I2S_FLAG_BSY) != 0);

	lcd_chipselect(1);
}

void __USER_TEXT lcd_write_data(uint8_t value)
{
	lcd_ctrllines_write(LCD_WRX_GPIO_PORT, LCD_WRX_PIN, 1);

	lcd_chipselect(0);
	spi_i2s_send(LCD_SPI, value);

	while (spi_i2s_get_flagstatus(LCD_SPI, SPI_I2S_FLAG_TXE) == 0);

	while (spi_i2s_get_flagstatus(LCD_SPI, SPI_I2S_FLAG_BSY) != 0);

	lcd_chipselect(1);
}

void __USER_TEXT lcd_poweron(void)
{
	uint8_t i;
	lcd_write_cmd(0xca);
	lcd_write_data(0xc3);
	lcd_write_data(0x08);
	lcd_write_data(0x50);
	lcd_write_cmd(0xcf); //LCD_POWERB
	lcd_write_data(0x00);
	lcd_write_data(0xc1);
	lcd_write_data(0x30);
	lcd_write_cmd(0xed); //LCD_POWER_SEQ
	lcd_write_data(0x64);
	lcd_write_data(0x03);
	lcd_write_data(0x12);
	lcd_write_data(0x81);
	lcd_write_cmd(0xe8); //LCD_DTCA
	lcd_write_data(0x85);
	lcd_write_data(0x00);
	lcd_write_data(0x78);
	lcd_write_cmd(0xcb); //LCD_POWERA
	lcd_write_data(0x39);
	lcd_write_data(0x2c);
	lcd_write_data(0x00);
	lcd_write_data(0x34);
	lcd_write_data(0x02);
	lcd_write_cmd(0xf7); //LCD_PRC
	lcd_write_data(0x20);
	lcd_write_cmd(0xea); //LCD_DTCB
	lcd_write_data(0x00);
	lcd_write_data(0x00);
	lcd_write_cmd(0xb1); //LCD_FRC
	lcd_write_data(0x00);
	lcd_write_data(0x1b);
	lcd_write_cmd(0xb6); //LCD_DFC
	lcd_write_data(0x0a);
	lcd_write_data(0xa2);
	lcd_write_cmd(0xc1); //LCD_Power2
	lcd_write_data(0x10);
	lcd_write_cmd(0xc5); //LCD_VCOM1
	lcd_write_data(0x45);
	lcd_write_data(0x15);
	lcd_write_cmd(0xc7); //LCD_VCOM2
	lcd_write_data(0x90);
	lcd_write_cmd(0x36); //LCD_MAC
	lcd_write_data(0xc8);
	lcd_write_cmd(0xf2); //LCD_3GAMMA_EN
    lcd_write_data(0x00);
    lcd_write_cmd(0xb0); //LCD_RGB_INTERFACE
	lcd_write_data(0xc2);
	lcd_write_cmd(0xb6); //LCD_DFC
	lcd_write_data(0x0a);
	lcd_write_data(0xa7);
	lcd_write_data(0x27);
	lcd_write_data(0x04);

	/* colomn address set */
	lcd_write_cmd(0x2a); //LCD_COLUMN_ADDR
	lcd_write_data(0x00);
	lcd_write_data(0x00);
	lcd_write_data(0x00);
	lcd_write_data(0xef);
	/* Page Address Set */
	lcd_write_cmd(0x2b); //LCD_PAGE_ADDR
	lcd_write_data(0x00);
	lcd_write_data(0x00);
	lcd_write_data(0x01);
	lcd_write_data(0x3f);
	lcd_write_cmd(0xf6); //LCD_INTERFACE
	lcd_write_data(0x01);
	lcd_write_data(0x00);
	lcd_write_data(0x06);

	lcd_write_cmd(0x2c); //LCD_GRAM
	//L4_Sleep(200);
	/* L4_Sleep unable to be used at here, so I use while temporary */
	/* TODO: A proper delay */
  	i = 0;
	while (i < 200)
		i++;

	lcd_write_cmd(0x26); //LCD_GAMMA
	lcd_write_data(0x01);
	
	lcd_write_cmd(0xe0); //LCD_PGAMMA
	lcd_write_data(0x0f);
	lcd_write_data(0x29);
	lcd_write_data(0x24);
	lcd_write_data(0x0c);
	lcd_write_data(0x0e);
	lcd_write_data(0x09);
	lcd_write_data(0x4e);
	lcd_write_data(0x78);
	lcd_write_data(0x3c);
	lcd_write_data(0x09);
	lcd_write_data(0x13);
	lcd_write_data(0x05);
	lcd_write_data(0x17);
	lcd_write_data(0x11);
	lcd_write_data(0x00);
	lcd_write_cmd(0xe1); //LCD_NGAMMA
	lcd_write_data(0x00);
	lcd_write_data(0x16);
	lcd_write_data(0x1b);
	lcd_write_data(0x04);
	lcd_write_data(0x11);
	lcd_write_data(0x07);
	lcd_write_data(0x31);
	lcd_write_data(0x33);
	lcd_write_data(0x42);
	lcd_write_data(0x05);
	lcd_write_data(0x0c);
	lcd_write_data(0x0a);
	lcd_write_data(0x28);
	lcd_write_data(0x2f);
	lcd_write_data(0x0f);
	lcd_write_cmd(0x11); //LCD_SLEEP_OUT
	//L4_Sleep(200);
	/* L4_Sleep unable to be used at here, so I use while temporary */
	/* TODO: A proper delay */
  	i = 0;
	while (i < 200)
		i++;
	lcd_write_cmd(0x29); //LCD_DISPLAY_ON
	/* GRAM start writing */
	lcd_write_cmd(0x2c); //LCD_GRAM
}

void __USER_TEXT lcd_spi_config(void)
{
	struct spi_cfg spi_init;
	/* Enable clock */
	RCC_AHB1PeriphClockCmd(LCD_SPI_SCK_GPIO_CLK | LCD_SPI_MISO_GPIO_CLK | LCD_SPI_MOSI_GPIO_CLK, 1);

	RCC_APB2PeriphClockCmd(LCD_SPI_CLK, 1);

	gpio_config_output(LCD_SPI_SCK_GPIO_PORT, LCD_SPI_SCK_PIN, GPIO_PUPDR_DOWN, GPIO_OSPEEDR_25M);
	gpio_config_output(LCD_SPI_MISO_GPIO_PORT, LCD_SPI_MISO_PIN, GPIO_PUPDR_DOWN, GPIO_OSPEEDR_25M);
	gpio_config_output(LCD_SPI_MOSI_GPIO_PORT, LCD_SPI_MOSI_PIN, GPIO_PUPDR_DOWN, GPIO_OSPEEDR_25M);

	gpio_pinaf_config(LCD_SPI_SCK_GPIO_PORT, LCD_SPI_SCK_PIN, af_spi5);
	gpio_pinaf_config(LCD_SPI_MISO_GPIO_PORT, LCD_SPI_MISO_PIN, af_spi5);
	gpio_pinaf_config(LCD_SPI_MOSI_GPIO_PORT, LCD_SPI_MOSI_PIN, af_spi5);

	spi_i2s_reset(LCD_SPI);

	if ((*(SPI_CR1(LCD_SPI)) & (SPI_CR1_SPE)) == 0) {
		// Cause memfault using declaration down here
		/*struct spi_cfg spi_init = {
			.direction = (uint16_t)0x0000, //2 lines full duplex
			.mode = (uint16_t) 0x0104, //master
			.size = (uint16_t) 0x0000, //8b
			.cpol = (uint16_t) 0x0000, //low
			.cpha = (uint16_t) 0x0000, //1Edge
			.nss = (uint16_t) 0x0200, //soft
			.baudrate = (uint16_t) 0x0018, //prescaler 16
			.firstbit = (uint16_t) 0x0000, //MSB
			.crcpolynomial = 7
		};*/
		spi_init.direction = (uint16_t)0x0000; //2 lines full duplex
		spi_init.mode = (uint16_t) 0x0104; //master
		spi_init.size = (uint16_t) 0x0000; //8b
		spi_init.cpol = (uint16_t) 0x0000; //low
		spi_init.cpha = (uint16_t) 0x0000; //1Edge
		spi_init.nss = (uint16_t) 0x0200; //soft
		spi_init.baudrate = (uint16_t) 0x0018; //prescaler 16
		spi_init.firstbit = (uint16_t) 0x0000; //MSB
		spi_init.crcpolynomial = 7;
		spi_config(LCD_SPI, &spi_init);
		/* Enable L3GD20_SPI */
		spi_cmd(LCD_SPI, 1);
	}
}

void __USER_TEXT lcd_ctrllines_write(uint8_t port, uint8_t pin, uint8_t reset)
{
	gpio_writebit(port, pin, reset);
}

void __USER_TEXT lcd_ctrllines_init(void)
{
	RCC_AHB1PeriphClockCmd(LCD_NCS_GPIO_CLK | LCD_WRX_GPIO_CLK, 1);
	gpio_config_output(LCD_WRX_GPIO_PORT, LCD_WRX_PIN, GPIO_PUPDR_NONE, GPIO_OSPEEDR_50M);
	gpio_config_output(LCD_WRX_GPIO_PORT, LCD_NCS_PIN, GPIO_PUPDR_NONE, GPIO_OSPEEDR_50M);

	lcd_ctrllines_write(LCD_NCS_GPIO_PORT, LCD_NCS_PIN, 1);
}

void __USER_TEXT lcd_chipselect(uint8_t enable)
{
	if (enable == 0) {
		*GPIO_BSRR(LCD_NCS_GPIO_PORT) = GPIO_BSRR_BR(LCD_NCS_PIN);
	} else {
		*GPIO_BSRR(LCD_NCS_GPIO_PORT) = GPIO_BSRR_BS(LCD_NCS_PIN);
	}
}

void __USER_TEXT lcd_af_gpio_init(void)
{
	struct gpio_cfg gpio_init;

	RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | \
                           RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN | \
						   RCC_AHB1ENR_GPIOFEN | RCC_AHB1ENR_GPIOGEN, 1);

	/* GPIOA */
	/* gpio_afr will be done too in gpio_config, since gpio_config *
	 * can't set pins at once, so I do it separately.              */
	
	/*gpio_pinaf_config(GPIOA, 3, af_ltdc);
	gpio_pinaf_config(GPIOA, 4, af_ltdc);
	gpio_pinaf_config(GPIOA, 6, af_ltdc);
	gpio_pinaf_config(GPIOA, 11, af_ltdc);
	gpio_pinaf_config(GPIOA, 12, af_ltdc);*/

	gpio_init.port = GPIOA;
	gpio_init.speed = GPIO_OSPEEDR_50M;
	gpio_init.pupd = GPIO_PUPDR_NONE;
	gpio_init.type = GPIO_MODER_ALT;
	gpio_init.func = af_ltdc;
	gpio_init.o_type = GPIO_OTYPER_PP;
	gpio_init.pin = 3;
	gpio_config(&gpio_init);
	gpio_init.pin = 4;
	gpio_config(&gpio_init);
	gpio_init.pin = 6;
	gpio_config(&gpio_init);
	gpio_init.pin = 11;
	gpio_config(&gpio_init);
	gpio_init.pin = 12;
	gpio_config(&gpio_init);
	
	/* GPIOB */
	gpio_init.port = GPIOB;
	gpio_init.func = 0x09;
	gpio_init.pin = 0;
	gpio_config(&gpio_init);
	gpio_init.pin = 1;
	gpio_config(&gpio_init);
	gpio_init.func = af_ltdc;
	gpio_init.pin = 8;
	gpio_config(&gpio_init);
	gpio_init.pin = 9;
	gpio_config(&gpio_init);
	gpio_init.pin = 10;
	gpio_config(&gpio_init);
	gpio_init.pin = 11;
	gpio_config(&gpio_init);

	/* GPIOC */
	gpio_init.port = GPIOC;
	gpio_init.pin = 6;
	gpio_config(&gpio_init);
	gpio_init.pin = 7;
	gpio_config(&gpio_init);
	gpio_init.pin = 10;
	gpio_config(&gpio_init);

	/* GPIOD */
	gpio_init.port = GPIOD;
	gpio_init.pin = 3;
	gpio_config(&gpio_init);
	gpio_init.pin = 6;
	gpio_config(&gpio_init);

	/* GPIOF */
	gpio_init.port = GPIOF;
	gpio_init.pin = 10;
	gpio_config(&gpio_init);

	/* GPIOG */
	gpio_init.port = GPIOG;
	gpio_init.pin = 6;
	gpio_config(&gpio_init);
	gpio_init.pin = 7;
	gpio_config(&gpio_init);
	gpio_init.func = 0x09;
	gpio_init.pin = 10;
	gpio_config(&gpio_init);
	gpio_init.func = af_ltdc;
	gpio_init.pin = 11;
	gpio_config(&gpio_init);
	gpio_init.func = 0x09;
	gpio_init.pin = 12;
	gpio_config(&gpio_init);

}

void __USER_TEXT lcd_layer_init(void)
{
	struct ltdc_layer_cfg layer_init;
	
	//Initialize Layer1
	layer_init.horizontal_start = 30;
	layer_init.horizontal_end = (LCD_PIXEL_WIDTH + 30 - 1);
	layer_init.vertical_start = 4;
	layer_init.vertical_end = (LCD_PIXEL_HEIGHT + 4 - 1);

	layer_init.pixel_format = LTDC_Pixelformat_RGB565;
	layer_init.constant_alpha = 255;

	layer_init.default_blue = 0;
	layer_init.default_green = 0;
	layer_init.default_red = 0;
	layer_init.default_alpha = 0;

	layer_init.blending_factor1 = LTDC_BlendingFactor1_CA;
	layer_init.blending_factor2 = LTDC_BlendingFactor2_CA;

	layer_init.cfb_line_length = ((LCD_PIXEL_WIDTH * 2) + 3);
	layer_init.cfb_pitch = (LCD_PIXEL_WIDTH * 2);
	layer_init.cfb_line_number = LCD_PIXEL_HEIGHT;
	layer_init.cfb_start_address = LCD_FRAME_BUFFER;
	
	ltdc_layer_cfg(LTDC_Layer1, &layer_init);

	// Initialize Layer2 
	layer_init.cfb_start_address = LCD_FRAME_BUFFER + BUFFER_OFFSET;
	
	layer_init.blending_factor1 = LTDC_BlendingFactor1_PAxCA;
	layer_init.blending_factor2 = LTDC_BlendingFactor2_PAxCA;

	ltdc_layer_cfg(LTDC_Layer2, &layer_init);

	//ltdc configuration reload
	ltdc_reload_cfg(LTDC_IMReload);

	// Enable Layers
	ltdc_layer_cmd(LTDC_Layer1, 1);
	ltdc_layer_cmd(LTDC_Layer2, 1);

	//ltdc configuration reload
	ltdc_reload_cfg(LTDC_IMReload);

	//set default font
	lcd_set_font(&LCD_DEFAULT_FONT);

	//dithering activation
	ltdc_dither_cmd(1);
}

void __USER_TEXT lcd_set_font(sFONT *fonts)
{
	lcd_current_font = fonts;
}
