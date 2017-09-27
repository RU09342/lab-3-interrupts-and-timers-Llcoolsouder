#include <msp430.h> 

/*
 * main.c
 *
 *  Created on: Sept 27, 2017
 *  Last Edited: Sept 27, 2017
 *      Author: Lonnie Souder II
 */


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	P1SEL &= ~(BIT0 | BIT3);    // Set P1.0 (LED) and P1.3 (Button) as GPIO
	P1SEL2 &= ~(BIT0 | BIT3);

    P1DIR &= ~BIT3;             // Set P1.3 (Button) as input
    P1DIR |= BIT0;              // Set P1.0 (LED) as output

	P1REN |= BIT3;              // Enable resistor on P1.3 (Button)
	P1OUT |= BIT3;              // Select pullup resistor

	P1IE |= BIT3;               // Enable interrupt on P1.3 (Button)
	P1IES |= BIT3;              // Set "Interrupt Edge Select" to falling edge
	P1IFG &= ~BIT3;             // Clear interrupt flag on P1.3 (Button)

	_BIS_SR(LPM4_bits + GIE);	// Enter Low Power Mode 4 and enable global interrupt
}

//Port 1 Interrupt Service Routine
#pragma vector = PORT1_VECTOR
__interrupt void PORT_1(void) {
	P1OUT ^= BIT0; 				// Toggle P1.0 (LED)
	P1IFG &= ~BIT3;
}

