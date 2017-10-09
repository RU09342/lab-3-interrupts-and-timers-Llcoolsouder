#include <msp430g2553.h>

/*
 * main.c
 *
 *  Created on: Oct 6, 2017
 *  Last Edited: Oct 6, 2017
 *      Author: Lonnie Souder II
 */

enum Edge {Rising_Edge, Falling_Edge};

int main(void)
{
    // NOTE:    Registers are cleared on power up
    //          This means bits need only to be set; not cleared.

	WDTCTL = WDTPW | WDTHOLD;		// stop watchdog timer
	
	//P1SEL &= ~(BIT1 + BIT0);		// Set P1.0 and P1.1 to GPIO
	//P1SEL2 &= ~(BIT1 + BIT0);
	P1DIR |= BIT0;					// Set P1.0 (LED) to output
	//P1OUT &= ~BIT0;			    // Initialize P1.0 to LOW
	P1REN |= BIT1;                  // Enable resistor on P1.1
	//P1OUT &= ~BIT1;               // Set pull down resistor on P1.1
	P1IE |= BIT1;                   //enable interrupt on P1.1
	//P1IES &= ~BIT1                // Interrupt on posedge of P1.1

	TACTL |=    TASSEL_2 +          // Select clock source, clk divider, clk mode, overflow interrupt
	            ID_1 +
	            MC_1 +
	            TAIV;
	TACCR0 = 0x61A8;				// Set clock period to 32000/8000000 [s] (10Hz)
	TACCTL0 |= CCIE;				// Enable Timer A capture/compare interrupt

	_BIS_SR(LPM1_bits + GIE);		// Enter Low Power Mode 1 and enable global interrupt

	return 0;
}

// Timer Interrupt Service Routine
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A(void) {
	P1OUT ^= BIT0; 					// Toggle P1.0 (LED)
	TACCTL0 &= ~CCIFG;
}

// P1.1 (Button) Service routine
#pragma vector = PORT1_VECTOR
__interrupt void PORT_1(void) {
    TACCR0 = 0x0000;
}
