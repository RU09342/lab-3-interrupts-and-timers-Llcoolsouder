# Button Based Delay
The following code basically combines Timer A Blink and Button Interrupt to create a system that records how long a button has been pushed and then sets the period of an LED blink to that amount of time. The following code is specifically for the MSP430G2553.

```
long int overflows;	// Keeps track of how many times TA overflows while Btn is pushed
long int count;	// Keeps track of how long its been since the LED has been toggled

enum Trigger {Rising_Edge, Falling_Edge};
enum Trigger edge = Falling_Edge;	//Keeps track of Btn pushes

int main(void)
{
    // NOTE:    Most registers are cleared on power up
    //          This means bits need only to be set; not cleared.

	WDTCTL = WDTPW | WDTHOLD;		// stop watchdog timer
	
	//P1SEL &= ~(BIT2 | BIT0);		// Set P1.0 and P1.2 to GPIO
	//P1SEL2 &= ~(BIT2 | BIT0);
	P1DIR |= BIT0;					// Set P1.0 (LED) to output
	P1OUT &= ~BIT0;			        // Initialize P1.0 to LOW
	P1REN |= BIT2;                  // Enable resistor on P1.2
	//P1OUT &= ~BIT2;               // Set pull down resistor on P1.2
	P1IE |= BIT2;                   // enable interrupt on P1.2
	P1IES &= ~BIT2;                 // Interrupt on posedge of P1.2

	TACTL |=    TASSEL_2 |          // Select clock source, clk divider, clk mode, overflow interrupt
	            ID_1 |
	            MC_1;

	TACCR0 = 0x61A8;				// Set clock period to 32000/8000000 [s] (10Hz)

	TACCTL0 |= CCIE;				// Enable Timer A capture/compare interrupt 0

	_BIS_SR(LPM1_bits | GIE);		// Enter Low Power Mode 1 and enable global interrupt

	return 0;
}

// Timer0 Interrupt Service Routine
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A(void) {
	if (edge == Rising_Edge){		// Btn is no longer pushed
		if (!(P1IN & BIT2)){
			edge = Falling_Edge;
			count = 0;
		}
		else {						// Count overflows while button is pushed
			overflows++;
		}
	}
	else {							// Button is no longer pushed
		if (count == overflows){	// Count equals the previous number of overflows
			P1OUT ^= BIT0;
			count = 0;
		}
		else
			count++;
	}
	TACCTL0 &= ~CCIFG;
}

// P1.2 (Button) Service routine
#pragma vector = PORT1_VECTOR
__interrupt void PORT_1(void) {
    overflows = 0;
    edge = Rising_Edge;
    P1IFG &= ~BIT2;
}
```

The code shown above doesn't implement any new features of the MSP430. What is shown here is simply a new combination of skills already learned that make up a new, novel system.
