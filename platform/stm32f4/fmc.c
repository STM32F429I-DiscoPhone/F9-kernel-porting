#include <platform/stm32f4/fmc.h>

void fmc_sdram_init(struct fmc_sdram_cfg *cfg)
{
	uint32_t tmpr1 = 0;
	uint32_t tmpr2 = 0;
	uint32_t tmpr3 = 0;
	uint32_t tmpr4 = 0;

	//TODO: assertion

	tmpr1 = (uint32_t)cfg->column_bits_number | 
	        cfg->row_bits_number |
	        cfg->sdmemory_data_width | 
			cfg->internal_bank_number |
			cfg->cas_latency | 
			cfg->write_protection | 
			cfg->sd_clock_period |
			cfg->readburst | 
			cfg->readpipe_delay;

	if (cfg->bank == FMC_Bank1_SDRAM) {
		*FMC_Bank5_6_SDCR(cfg->bank) = tmpr1;
	} else {
		tmpr3 = (uint32_t)cfg->sd_clock_period |
		        cfg->readburst |
				cfg->readpipe_delay;

		*FMC_Bank5_6_SDCR(FMC_Bank1_SDRAM) = tmpr3;
		*FMC_Bank5_6_SDCR(cfg->bank) = tmpr1;
	}

	if (cfg->bank == FMC_Bank1_SDRAM) {
		tmpr2 = (uint32_t)((cfg->timing->lta_delay)-1) |
		        (((cfg->timing->esr_delay)-1) << 4) |
				(((cfg->timing->sr_time)-1) << 8) |
				(((cfg->timing->rc_delay)-1) << 12) |
				(((cfg->timing->wr_time)-1) << 16) |
				(((cfg->timing->rp_delay)-1) << 20) |
				(((cfg->timing->rcd_delay)-1) << 24);

		*FMC_Bank5_6_SDTR(cfg->bank) = tmpr2;
	} else {
		tmpr2 = (uint32_t)((cfg->timing->lta_delay)-1) |
		        (((cfg->timing->esr_delay)-1) << 4) |
				(((cfg->timing->sr_time)-1) << 8) |
				(((cfg->timing->wr_time)-1) << 16);

		tmpr4 = (uint32_t)(((cfg->timing->rc_delay)-1) << 12) |
		        (((cfg->timing->rp_delay)-1) <<20);

		*FMC_Bank5_6_SDTR(FMC_Bank1_SDRAM) = tmpr4;
		*FMC_Bank5_6_SDTR(cfg->bank) = tmpr2;
	}

}
