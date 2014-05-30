#include <platform/stm32f4/ltdc.h>

#define GCR_MASK                    ((uint32_t)0x0FFE888F)
									
void __USER_TEXT ltdc_init(struct ltdc_cfg *cfg)
{
	uint32_t horizontalsync = 0;
	uint32_t accumulatedHBP = 0;
	uint32_t accumulatedactiveW = 0;
	uint32_t totalwidth = 0;
	uint32_t backgreen = 0;
	uint32_t backred = 0;

	//TODO: assertion

	// synchronization size
	*LTDC_SSCR &= ~(LTDC_SSCR_VSH | LTDC_SSCR_HSW);
	horizontalsync = (cfg->horizontal_sync << 16);
	*LTDC_SSCR |= (horizontalsync | cfg->vertical_sync);

	// accumulated back porch
	*LTDC_BPCR &= ~(LTDC_BPCR_AVBP | LTDC_BPCR_AHBP);
	accumulatedHBP = (cfg->accumulated_hbp << 16);
	*LTDC_BPCR |= (accumulatedHBP | cfg->accumulated_vbp);

	// accumulated active width
	*LTDC_AWCR &= ~(LTDC_AWCR_AAH | LTDC_AWCR_AAW);
	accumulatedactiveW = (cfg->accumulated_active_w << 16);
	*LTDC_AWCR |= (accumulatedactiveW | cfg->accumulated_active_h);

	// total width
	*LTDC_TWCR &= ~(LTDC_TWCR_TOTALH | LTDC_TWCR_TOTALW);
	totalwidth = (cfg->total_width << 16);
	*LTDC_TWCR |= (totalwidth | cfg->total_height);

	*LTDC_GCR &= (uint32_t)GCR_MASK;
	*LTDC_GCR |= (uint32_t)(cfg->hs_polarity | cfg->vs_polarity | \
	             cfg->de_polarity | cfg->pc_polarity);

	// background color
	backgreen = (cfg->bg_green_value << 8);
	backred = (cfg->bg_red_value << 16);

	*LTDC_BCCR &= ~(LTDC_BCCR_BCBLUE | LTDC_BCCR_BCGREEN | LTDC_BCCR_BCRED);
	*LTDC_BCCR |= (backred | backgreen | cfg->bg_blue_value);
}
