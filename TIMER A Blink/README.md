# TIMER A Blink
In this part of the lab, instead of using ```__delay_cycle(const int)``` or a while loop delay, I utilize the MSP430's built in timer modules to generate more precise delays that trigger on interrupts. The code shown below is specifically for the MSP430G2553.

```
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
```
## Timer A
This is the first lab that uses Timer A. The 3 important registers in this lab are TA0CTL, TA0CCR0, and TA0CCTL0. TA0CTL contains information that cotrols the main timer. Through this register one can select the clock source for the timer, set a clock divider, set the timer's counting mode, and many more things. TA0CCR0 is a capture/compare register. Timer A has more than one capture/compare register, but CCR0 is special. CCR0 sets the max value when counting in Up Mode or Up/Down Mode. More on that later. On any other CCRn, this register is used to either store capture values or set the number at which the module will trigger a compare interrupt. TA0CCTL0 is the capture/compare control register. This register is used to control things such as capture/compare mode, capture edge select, capture/compare interrupt enable and many more things.

## Up, Down, Continuous 
There are a few different ways that the timer module can count. For starters, one of the easiest to initialize is Continuous counting where in the TIMER module will alert you when its own counting register overflows. Up mode allows you to utilize a Capture/Compare register to have the counter stop at a particular count and then start back over again. You can also set the TIMER to Up/Down mode where upon hitting a counter or the overflow, instead of setting the counter back to zero, it will count back down to zero. 
