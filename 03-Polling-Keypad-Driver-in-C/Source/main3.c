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
ui_16 val1, val2, total;
// System Initialisation
Initialise();
// Peripheral Initialisation
keypadInit();
LEDsInit();
sevenSegInit();

for(;;) {
	if(keypadGet(&temp))
	{
		if (i == 1)
		{
			val1 = ~temp;
			flag = LEDsPut(~temp);
			flag = sevenSegPut(DispID1, val1);	
			i = 2;		
		}
		else if (i == 2)
		{
			val2 = ~temp;
			flag = LEDsPut(~temp);
			flag = sevenSegPut(DispID1, val2);
			i = 1;
		}
		//total = val1 + val2;
		//flag = sevenSegPut(DispID1, total);
		//flag = sevenSegPut(DispID2, total);
		//i = 1;

	}
} /* loop forever */
  /* please make sure that you never leave main */
}
