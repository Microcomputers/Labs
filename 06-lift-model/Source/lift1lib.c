/* A library of functions for use with one lift simulator and the MSP430 CLIC3 board.
For use in polling based programs.
 C.A.Maynard 20140703 v1.2
 */
#include "clic3.h"
#include "lift1lib.h"

/* Globals for general lift program.
Declare as external when used in the main program
*/
extern ui_16 BusAddress,BusData;
uc_8 Stimulus1, LedData1, CommandLine1[16];
uc_8 Lift1, MovingStatus1; /* Holds motor control info  */

/* Initialisation */
void liftInit(void)
{ 
	P1IES&=~0x03;             /*Detect Rising edge of the Level & Button interrupt*/
//	P1IE|=0x03;               /*Do not.... Enable interrupts */
}

/* Functions to control the lift movement 
Returns true for all cases.
CAM 20120731
Modified for MSP430 CLIC3 board
CAM 20140703
*/ 
enum bool lift1Up (void)
{
  MovingStatus1=motorUp;
  Lift1 = motorUp;
  BusAddress=Lift1Address;  
  BusData=Lift1;
  BusWrite();
  Stimulus1=1;
  return true;
}

enum bool lift1Down (void)
{
  MovingStatus1 = motorDown;
  Lift1=motorDown;
  BusAddress=Lift1Address;  
  BusData=Lift1;
  BusWrite();    
  Stimulus1=0;
 return true;
}

enum bool lift1Stop (void)
{
  MovingStatus1 = motorStop;
  Lift1=motorStop;
  BusAddress=Lift1Address;  
  BusData=Lift1;
  BusWrite();
  return true;
}
/**********************************************************************/

/* Function to switch a specific LED on or off 
LEDSelect must be in the range 0x00 to 0x0F (or 0 to 15 decimal) 
LEDState can take two values LEDOn or LEDOff 
Always returns true. Some delay needed between writes to bus!
*/
enum bool WriteLed1 (uc_8 LEDAddress1, uc_8 LEDData1)
{
  char LedByte1;
  si_32 DelayConst1;
  LedByte1=LEDAddress1|LEDData1|MovingStatus1;
  BusAddress=Lift1Address;  
  BusData=LedByte1;
  BusWrite();
  DelayConst1=100;
  Delay1(DelayConst1);
  LedByte1=LedByte1|LEDLatchOn;            /*Activate Latch signal*/
  BusData=LedByte1;
  BusWrite();
  Delay1(DelayConst1);
  LedByte1=LedByte1&LEDLatchOff;           /*Mask Latch signal*/
  BusData=LedByte1;
  BusWrite();
  Delay1(DelayConst1);
  return true;
}

/**********************************************************************/
// Functions to Open and Close the lift doors. Delays included
void OpenDoor1 (void){
CommandLine1[7]=0;
WriteLed1(5, LEDOn);
Delay1(6000000);
LedData1=LEDOff;
WriteLed1(7, LedData1);
}

void CloseDoor1 (void){
CommandLine1[6]=0;
LedData1=LEDOff;
WriteLed1(5, LedData1);
Delay1(6000000);
WriteLed1(6, LedData1); 
}

/**********************************************************************/
/* Function to check if a button has been pushed.
Returns true if a button has been pushed and with the button ID
otherwise returns false.
Modified for CLIC3 CAM 20140703
*/
enum bool buttonGet (uc_8 *buttonID)
{	enum bool buttonPushed = false;
uc_8 Lift;
if (P1IFG & buttonBit) /* The bit has been set */
{    BusAddress=Lift1Address;  
     BusRead();
     Lift=BusData;
     *buttonID = Lift>>4;
     P1IFG &= ~buttonBit ; // Clear the bit
     buttonPushed = true;}

return (buttonPushed);
}


/* Function to check if the cage is at a floor
Returns true if at a floor and with the floor ID
otherwise false.
Modified for CLIC3 CAM 20140703
*/
enum bool floorGet (uc_8 *floorID)
{	enum bool floorSet = false;
uc_8 Lift;
if (P1IFG & floorBit) /* The bit has been set */
{    BusAddress=Lift1Address;  
     BusRead();
     Lift=BusData;
     *floorID = (Lift>>1) & floorMask;
     P1IFG &= ~floorBit; // Clear the bit
     floorSet = true; }
return (floorSet); 	
}


/**********************************************************************/
// Software time delay for simplicity here.
// Assume one cycle of the loop takes approximately 0.1microsecs  0.0001 mS * x ==> 100 mS
/*
void  Delay1(ui_32 DelayConst1){
   ui_32 i1;
   for (i1=0;i1<DelayConst1;i1++){}
}
*/
//100 ms delay
void  Delay1()
{
	ui_32 DelayConst1 = 1000000;
	ui_32 i1;
	for (i1=0;i1<DelayConst1;i1++){}
}

void Delay2(int count)
{
	for (int i = 0; i < count; ++i)
	{
		Delay1();
	}
}

int LEDAddress[] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd};

void turnLEDsON()
{
	for (int i = 0; i < sizeof(LEDAddress); ++i)
	{
		WriteLed1(i, LEDon);
	}
}

void turnLEDsOFF()
{
	for (int i = 0; i < sizeof(LEDAddress); ++i)
	{
		WriteLed1(i, LEDoff);
	}
}