/* The Third program for the CLIC3 board. CAM20140628
Monitor the keypad and update the LEDs display and the Seven Segment displays
******************************************************************** */
#include "clic3.h"
uc_8 temp;
ui_16 counter;
void main(void) {
// uc_8 temp;
enum bool flag;

// System Initialisation
Initialise();
// Peripheral Initialisation
keypadInit();
LEDsInit();
sevenSegInit();

for(;;) {
	if(keypadGet(&temp))
	{
		flag = LEDsPut(~temp);
		flag = sevenSegPut(DispID1, temp);
		flag = sevenSegPut(DispID2, temp);
	}
} /* loop forever */
  /* please make sure that you never leave main */
}
