#include <msp430g2553.h>

/*
 * main.c
 *
 *  Created on: Oct 6, 2017
 *  Last Edited: Oct 6, 2017
 *      Author: Lonnie Souder II
 */

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;				// stop watchdog timer
	
	P1SEL &= ~BIT0;							// Set P1.0 to GPIO
	P1SEL2 &= ~BIT0;
	P1DIR |= BIT0;							// Set P1.0 (LED) to output
	P1OUT &= ~BIT0;							// Initialize P1.0 to LOW

	// NOTE: 	Timer A registers are cleared on power up
	//			This means bits need only to be set; not cleared.

	TACTL |= TASSEL_2 + ID_1 + MC_1;		// Select clock source, clk divider, clk mode
	TACCR0 = 0x7D00;						// Set clock period to 32000/8000000 [s]
	TACCTL0 |= CCIE;						// Enable Timer A capture/compare interrupt

	_BIS_SR(LPM1_bits + GIE);				// Enter Low Power Mode 1 and enable global interrupt

	return 0;
}

	//Timer Interrupt Service Routine
	#pragma vector = TIMER0_A0_VECTOR
	__interrupt void Timer_A(void) {
		P1OUT ^= BIT0; 						// Toggle P1.0 (LED)
		TACCTL0 &= ~CCIFG;
	}
