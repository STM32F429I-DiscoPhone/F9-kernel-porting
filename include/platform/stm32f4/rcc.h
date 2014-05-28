#ifndef PLATFORM_STM32F4_RCC_H__
#define PLATFORM_STM32F4_RCC_H__

void sys_clock_init(void);
void RCC_AHB1PeriphClockCmd(uint32_t rcc_AHB1, uint8_t enable);
void RCC_AHB3PeriphClockCmd(uint32_t rcc_AHB3, uint8_t enable);
void RCC_APB2PeriphClockCmd(uint32_t rcc_APB2, uint8_t enable);
void RCC_APB2PeriphResetCmd(uint32_t rcc_APB2, uint8_t enable);

#endif
