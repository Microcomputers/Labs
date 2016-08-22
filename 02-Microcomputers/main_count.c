/* First program for the CLIC3 board. CAM20140530
Calls the initialisation and operational functions for the devices being used.
Input the switch settings.
Invert the switch bits and display on the LEDs
Output displayable switch values to the Seven Segment displays.
NOTE the boolean flags are not used in the first laboratory.
Uses:
clic3.h v3.02, clic3.c v3.01
initialise.asm v1.0, BusRead v1.3, BusWrite v2.3
******************************************************************** */

#include "clic3.h"

const int Seg1 = 0;
const int Seg2 = 1;

uc_8 temp; // Global for debug testing only
//uc_8 BIT7 = 10000000; //BIT7 is a defined macro

void main(void) {

enum bool flag1, flag2, flag3, flag4;
uc_8 count = 0; // counter
  
Initialise(); // System initialisation
switchesInit(); // Switches initialisation
LEDsInit(); // LED initialisation
sevenSegInit(); // 7segment initialisation

flag = LEDsPut(~count); // display initial value

	for(;;) {
		if (switchesGet(&temp))
		{
			count++; //increment counter
		    flag1 = switchesGet(&temp);
		    flag2 = LEDsPut(~count);
		    flag3 = sevenSegPut(Seg1, count);
		    flag4 = sevenSegPut(Seg2, count);
		}
		for (int i = 0; i < 100; ++i)
		{
			for (int j = 0; j < 1000; ++j)
			{
				// Software time delay to make operations visible
			}
		}
	} /* loop forever */
}
