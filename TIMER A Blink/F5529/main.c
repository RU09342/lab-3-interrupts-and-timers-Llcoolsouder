#include <msp430g2553.h>

/*
 * main.c
 *
 *  Created on: Oct 6, 2017
 *  Last Edited: Oct 6, 2017
 *      Author: Lonnie Souder II
 */


// Function to initiate TA0 with a specific frequency in Hz
// Assume TA0CTL = TASSEL_2 + MC_1 + ID_2
void setTimerFrequency(int f)
{
    int n;
    n = 250000 / f; //250000 / 10 = 25000
    TA0CCR0 = n; // [( 10^6 / 4) / (25000) = 10Hz)]
}

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
	//TACCR0 = 0x7D00;						// Set clock period to 32000/250000 [s]
	setTimerFrequency(10);
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
