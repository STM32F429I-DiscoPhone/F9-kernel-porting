#ifndef PLATFORM_STM32F4_LCD_H__
#define PLATFORM_STM32F4_LCD_H__

#include <platform/link.h>
#include <platform/stm32f4/registers.h>
#include <platform/stm32f4/fonts.h>

#define LCD_PIXEL_WIDTH     ((uint16_t)240)
#define	LCD_PIXEL_HEIGHT    ((uint16_t)320)

#define	LCD_FRAME_BUFFER	((uint32_t)0xD0000000)
#define	BUFFER_OFFSET		((uint32_t)0x50000)

#define LCD_NCS_PIN			2
#define	LCD_NCS_GPIO_PORT	GPIOC
#define	LCD_NCS_GPIO_CLK	RCC_AHB1ENR_GPIOCEN
#define LCD_NCS_PIN			2

#define LCD_WRX_GPIO_PORT	GPIOD
#define LCD_WRX_GPIO_CLK	RCC_AHB1ENR_GPIODEN
#define LCD_WRX_PIN			13

#define LCD_SPI_SCK_PIN		7
#define	LCD_SPI_SCK_GPIO_PORT	GPIOF
#define	LCD_SPI_SCK_GPIO_CLK	RCC_AHB1ENR_GPIOFEN
#define	LCD_SPI_MISO_PIN	8
#define	LCD_SPI_MISO_GPIO_PORT	GPIOF
#define	LCD_SPI_MISO_GPIO_CLK	RCC_AHB1ENR_GPIOFEN
#define	LCD_SPI_MOSI_PIN	9
#define	LCD_SPI_MOSI_GPIO_PORT	GPIOF
#define	LCD_SPI_MOSI_GPIO_CLK	RCC_AHB1ENR_GPIOFEN
#define LCD_SPI                 SPI5_BASE
#define LCD_SPI_CLK             RCC_APB2ENR_SPI5EN

#define LCD_DEFAULT_FONT        Font16x24

void lcd_init(void);
void lcd_write_cmd(uint8_t lcdreg);
void lcd_write_data(uint8_t value);
void lcd_poweron(void);
void lcd_spi_config(void);
void lcd_ctrllines_write(uint8_t port, uint8_t pin, uint8_t reset);
void lcd_ctrllines_init(void);
void lcd_chipselect(uint8_t enable);
void lcd_af_gpio_init(void);
void lcd_layer_init(void);
void lcd_set_font(sFONT *fonts);

#endif

