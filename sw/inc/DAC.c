/**
 * @file DAC.c
 * @author your name (you@domain.com)
 * @brief Low level driver for the DAC. Documentation comments are specifically
 *        for the TLV5618 12-bit SPI DAC. Modify for your particular DAC.
 * @version 0.3.0
 * @date 2023-02-14
 *
 * @copyright Copyright (c) 2023
 * @note Reference datasheet:
 *      https://www.ti.com/lit/ds/symlink/tlv5618a.pdf?ts=1676400608127&ref_url=https%253A%252F%252Fwww.google.com%252F
 */

#include "./DAC.h"
#include "./tm4c123gh6pm.h"

int dac_init() {
    /**
     * Unified_Port_Init in Lab5.c calls Port_D_Init, which initializes the Port
     * D GPIOs for the appropriate alternate functionality (SSI).
     *
     * According to Table 15-1. SSI Signals in the datasheet, this corresponds
     * to SSI1. The corresponding Valvanoware register defines are at L302 and
     * L2670 in inc/tm4c123gh6pm.h. Use this in combination with the datasheet
     * or any existing code to write your driver! An example of how to
     * initialize SSI is found in L741 in inc/ST7735.c.
     */
	
		// Turn on SSI1
		SYSCTL_RCGCSSI_R |= 0x02;
	
		SYSCTL_RCGCGPIO_R     |= 0x08;            // activate port D
		while((SYSCTL_PRGPIO_R & 0x08)==0){};     // allow time for clock to stabilize
	
	  // ---------------  Initialize PD3,1,0 as SSI1 MOSI, FS & SCK  ---------------- 
			
		GPIO_PORTD_AMSEL_R    &= ~0x0B;           // disable analog functionality on PD
		GPIO_PORTD_AFSEL_R    |=  0x0B;           // enable alt funct on PD3,1,0
		GPIO_PORTD_DEN_R      |=  0x0B;           // enable digital I/O on PD3,1,0
		GPIO_PORTD_PCTL_R      = (GPIO_PORTD_PCTL_R & 0xFFFF0F00)+ 0x00002022;
			
		SSI1_CR1_R = 0x00000000; // disable SSI, master mode
		SSI1_CPSR_R = 0x02; // 8 MHz SSIClk
	  SSI1_CR0_R &= ~(0x0000FFF0); // SCR = 0, SPH = 0, SPO = 0 Freescale
		SSI1_CR0_R |= 0x0F; // DSS = 16-bit data
		SSI1_DR_R = 0x0; // initially low output
		SSI1_CR1_R |= 0x00000002; // enable SSI
			
    return 0;
}

int dac_output(uint16_t data) {
    // An example of how to send data via SSI is found in L534 of inc/ST7735.c.
    // Remember that 4 out of the 16 bits is for DAC operation. The last 12 bits
    // are for data. Read the datasheet!
	
		while((SSI1_SR_R & 0x02) == 0){};
		SSI1_DR_R = 0x0FFF & data;							//For tlv5616 first 4-bits are control
			
    return 0;
	}
