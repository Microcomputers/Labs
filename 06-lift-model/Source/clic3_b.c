//Function code for 7 segment displays, switches and LEDs
// Simplest form of function for these devices.
// In the initial version of CLIC3 the Keypad operations are STUBS CAM 2014-06-02
// Conforms to MISRA C required rules except 109, 110

// C.A.Maynard 20140530 v3.00
// Updated 7 segment C.A,Maynard 20140627 v3.01

// Monitoring the keypad flag. Once per keypress!!
// C.A.Maynard 20140716 v3.1
// ************************************************************************

#include "clic3.h"
extern ui_16 counter;
// ************************************************************************

ui_16 BusAddress,BusData;
 // The bridge between Assembler code and C code.
// ************************************************************************

     
// General Interface Initialisation functions
void Initial(void);
void BusRead(void);
void BusWrite(void);
// ************************************************************************
// Local prototypes
enum bool keyCount(uc_8 *value);
// ************************************************************************

void switchesInit(void) {
}

// Returns true if a new value has been found
// This version returns a first value and subsequent changes.
// No change returns false.
// C.A.Maynard V3.00
enum bool switchesGet(uc_8 *value){
	enum bool flag = false; // Note initialised every time the function is entered.
	static enum bool first = true; // Initialised only at the start of execution
	static uc_8 old;
	uc_8 new;
    BusAddress=SwitchesAddr;
    BusRead();
	new = BusData; // Get the switch value
	if (first) {
		old = new;
		*value = old;
		flag = true;
		first = false;
	} else {
		if (old != new) {
			old = new;
			*value = old;
			flag = true;
		}
	}
	return flag;
}
// ************************************************************************

void LEDsInit(void) {
}

// Returns true if a new value can be stored in the interface
// As there is no way for the system to know just return true.
enum bool LEDsPut(uc_8 value){
    BusAddress=LedsAddr;
    BusData=value;
    BusWrite();
    return true;
}
// ************************************************************************
// In the initial version of CLIC3 the Keypad operations are STUBS CAM 2014-06-02
// Lookup table for operation of the Keypad
#define MaxKeys (16)
#define MaxScan (4)
const uc_8 LookupKeys[MaxKeys]={ 0x82,0x11,0x12,0x14,0x21,0x22,0x24,0x41,0x42,0x44,0x81,0x84,0x88,0x48,0x28,0x18 };
const uc_8 scan[MaxScan] = {0x10,0x20,0x40,0x80};
// Initialise the keypad input interface using the keypad flag as the test for key press.
void keypadInit(void){
P2IES&=~0x01;                            // Positive edge of the pin interrupt
// P2IE|=0x01;                             // Enable P2.0 interrupt
// P1IFG=0x00;                             // Clear all P1 interrupt flags  
P2IFG&=~0x01;                           // Clear P2.0 interrupt flag  

}

// Function to find a keypress value.
// Internal to the library NOT seen by user
// C.A.Maynard 20140716 v3.1
enum bool keyCount( uc_8 *value)
{ uc_8 temp1, Count = 0;
enum bool flag = false;
if(P2IFG & 0x01) {
	BusAddress = KeyPadAddr;
	BusRead();
	temp1 = BusData;
	while( !flag ) //Terminates on a valid code. No range checking!!!
  		{ if( temp1 == LookupKeys[ Count ] )
			{ *value = Count;
				flag = true;
			} else { 
        		Count++;
        		if(Count>15) {break;}
    }
  }
  P2IFG&=~0x01; // Clear keypad flag
}
return flag;
}

#define bouncelimit (200)
#define keyidle (0)
#define keyactive (1)
// Get a new value from the keypad if there is one otherwise return false
// Returns true if a new value has been found
// Simple state machine to ensure key release and timeout before a new one can be detected.
// Timer functionality is hardware dependent. Original for S12
// Modified for MSP430 assuming TA1 has been activated in Initialise to approx 1msec
// C.A.Maynard 20140629 v3.00
enum bool keypadGet(uc_8 *value) {
static uc_8 state = keyidle, keycode; // Start in idle
static ui_16 bouncecount = 0;
enum bool flag = false;

switch (state) {
  case keyidle: // Idle state - no key pressed
    if (keyCount(&keycode)) { // detect new keypress
      state = keyactive;
      *value = keycode;
      flag = true;
      // Start debounce timeout count
	  if(TA1CTL_bit.TAIFG == 1) {
	  	TA1CTL_bit.TAIFG = 0; // Clear any timer overflow
	  }
    } else {} // for MISRA-C
    break;
  case keyactive: // Active state - key already returned
  	// Increment timout count if overflow has occurred
    if(TA1CTL_bit.TAIFG == 1) {
	  	TA1CTL_bit.TAIFG = 0; // Clear any timer overflow
	  	bouncecount++;
	  }
	  // If the keybounce timelimit has been exceeded and there is no key pressed 
	  // clean up and return to idle
	if((bouncecount > bouncelimit)&&(!keyCount(&keycode))){
	  state = keyidle;
	  bouncecount = 0;
	} else {} // For MISRA-C!!!
    break;
  default: // Should not get here
     state = keyidle;
     break;
  }
  return flag;
}
// ************************************************************************

// Lookup table for operation of the 7segment displays
#define MaxSegs (0x1C)
#define Blank7Seg (0x1B) // Display blank index for the 7 segment display

const uc_8 LookupSeg[MaxSegs]={ 0x040,0x079,0x024,0x030,0x019,0x012,0x002,0x078,0x000,0x018,0x008,0x003,0x046,0x021,0x006,0x00E,
0x027, 0x009, 0x07B, 0x060, 0x047, 0x023, 0x00C, 0x02F, 0x041, 0x011, 0x03F, 0x07F};
/* Beyond index 0x0F (for the hex character "F")
the values represent the characters: c , H , i , J , L , o , P , r , U , Y , - , blank
 With hexadecimal indices           10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 1A, 1B
 */

// Initialise the Seven Segment Display output interface
void sevenSegInit(void){
}

/* Send a new value to the selected display and return true otherwise return false if this could not be achieved. 
DispID chooses the display (o or 1). 
value is the selector for what is to be displayed. 
If the result is out of range then blank the display.
*/
enum bool sevenSegPut(uc_8 DispID,uc_8 value) {
    if(DispID) {
    	BusAddress = Seg7AddrL; }
    else {
    	BusAddress = Seg7AddrH; }
    if(MaxSegs > value) {
        BusData = LookupSeg[value];
        BusWrite();
       return true;
    } else {
        BusData = LookupSeg[Blank7Seg];
        BusWrite();
        return false;
  }
}
// ************************************************************************
