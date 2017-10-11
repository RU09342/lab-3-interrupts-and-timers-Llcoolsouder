# Button Interrupt
In this part of the lab, I use a new method to handle inputs. In previous labs, the _polling_ technique was used. However, this takes a lot of processing power (CPU cycles) and a lot of power (Watts) in general. Instead this lab uses _interrupts_ to handle inputs immediately without the processor running constant checks on them. This is allows the processor to 1.) run in low power mode, and 2.) run other code while interrupts are not being serviced. The code below is specifically for the MSP430G2553. It was written to put the processor in "sleep mode" until a button is pushed, at which point, an LED is toggled.

```
#include <msp430.h> 

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	  // stop watchdog timer
	
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
```
As you have probably already noticed, this code sets 3 new registers: ```P1IE```, ```P1IES```, and ```P1IFG```; P1IE is stands for "Port 1 Interrupt Enable," and it does exactly that. It enables an interrupt on Port 1. P1IES stands for "Port 1 Interrupt Edge Select." This register is used to choose which transition on a particular pin will trigger an interrupt (low to high or high to low). P1IFG stands for "Port 1 Interrupt Flag." An interrupt is triggered when this is set. More on this later.

The other difference shown in the above code is the following line:
```
_BIS_SR(LPM4_bits + GIE);
```
This function call sets the processor to Low Power Mode 4 which essentially leaves the processor idle until an interrupt is triggered. It also sets ```GIE``` which stands for "General Interrupt Enable." Without this set, no interrupts will trigger.

## Interrupt Service Routines
Interrupt service routines are the code that is executed upon triggering an interrupt. The basic form for them is as follows.
```
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
  // Do something
  P1IFG &= ~BIT3;
}
```
It is very important to clear the interrupt flag during the interrupt service routine. Otherwise, the processor may continue to trigger interrupts which will cause the processor to get stuck on the same interrupt indefinitely.
