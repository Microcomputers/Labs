/* The Third program for the CLIC3 board. CAM20140628
Monitor the keypad and update the LEDs display and the Seven Segment displays
******************************************************************** */
#include "clic3.h"
uc_8 temp;
ui_16 counter;
int i = 1, j = 0;
void main(void) {
// uc_8 temp;
enum bool flag;
//keypad values
uc_8 val1, val2, total;
//over flow segments
uc_8 lSeg, rSeg;
// System Initialisation
Initialise();
// Peripheral Initialisation
keypadInit();
LEDsInit();
sevenSegInit();

for(;;) {
	if(keypadGet(&temp))
	{
		if (temp <= 9)	
		{
			if (i == 1)
			{
				val1 = temp;
				flag = LEDsPut(~temp);
					
				i = 2;		
			}
			else if (i == 2)
			{
				val2 = temp;
				flag = LEDsPut(~temp);
				
				i = 3;
			}
			else if (i == 3)
			{
				//stuff to reset the displays
				i = 1;
			}
			total = val1 + val2;
			if (total < 10)
			{
				total = total / 10;
				flag = sevenSegPut(DispID2, 0);
				flag = sevenSegPut(DispID1, total);
			}
			else if (total >= 10)
			{
				lSeg = total / 10;
				rSeg = total % 10;
				flag = sevenSegPut(DispID2, rSeg);
				flag = sevenSegPut(DispID1, lSeg);
			}
		}

		//total = val1 + val2;
		//flag = sevenSegPut(DispID1, total);
		//flag = sevenSegPut(DispID2, total);
		//i = 1;

		}
	} /* loop forever */
  /* please make sure that you never leave main */
}
