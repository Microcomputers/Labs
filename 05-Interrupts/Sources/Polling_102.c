/* Program to introduce polling using library functions for the interfaces
Standardising the form of interface function
C.A.Maynard v1.00 20101124
Conforms with MISRA C required rules except 109, 110.

Uses a timed interval for the polling process.
Set at approximately 2msecs
C.A.Maynard v1.01 20110127

Do input sampling in the interrupt routine (Foreground task).
Do output displays in the main loop (Background task)
Requires global variables for the data collection.
C.A.Maynard v1.02 20110127

Modified to work with MSP430 timer interrupt doing the polling.
C.A.Maynard 20140716 v2.0
Uses:
Busread, BusWrite, Initialise, clic3_b.c
*/
#include "clic3.h"

// Global variables for transfer from ISR to main program
enum bool switchesFlag = false, keypadFlag = false;
uc_8 switchesValue,keypadValue;

// Interrupt prototype declaration
#pragma vector=TIMER1_A1_VECTOR
__interrupt void TIMER1_A1_ISR( void );

void main (void) 
{
	uc_8 segID = 0;
	enum bool temp;

	// System initialisation
	Initialise();
	// Peripheral initialisation
	switchesInit();
	LEDsInit();
	sevenSegInit();
	keypadInit();
	// ****************************************************************
	// Acivate the probe display connection
	P4DIR |=BIT3; // Get a probe point
	P4OUT &= ~BIT3; // clear it to start low
	// Activate the Timer1 overflow interrupt for polling
    TA1CTL_bit.TAIFG = 0; // clean up any pending interrupts
    TA1CTL_bit.TAIE = 1; // enable the interrupt for the timer
	// ****************************************************************

	__enable_interrupt(); // Allow the interrupt to work
	// main loop (Background task)
	while (true) 
	{
		// Get the switch input and display
		if( switchesFlag) 
		{
			temp = LEDsPut(~switchesValue); // the return value is not used in this code.
			switchesFlag = false;
		}
	    // Get the keypad input and display
		if(keypadFlag ) 
		{
			temp = sevenSegPut(segID,keypadValue); // the return value is not used in this code.
			segID ^= 0x01;
			keypadFlag = false;
		}

	   // Anything else to be done
	} // end while (true)
}

__interrupt void TIMER1_A1_ISR( void )
{ // Foreground task get any inputs every 1msecs
	P4OUT ^= BIT3; // CRO display
	switchesFlag = switchesGet(&switchesValue);
	keypadFlag = keypadGet(&keypadValue);
	TA1CTL_bit.TAIFG = 0;
	P4OUT ^= BIT3;
}
