/* Header file for a library of functions for use with one lift simulator 
and the MSP430 CLIC3 board. For use in polling based programs. 
 C.A.Maynard 20140703 v1.2
 */
#ifndef liftlibdefs
#define liftlibdefs

#include "clic3.h"

 /* ---- Clic3 Magic Numbers ----- */
#define bit7Set   0x080
#define floorMask 0x007

#define floorBit  0x001
#define buttonBit 0x002

#define motorUp   0x080
#define motorDown 0x040
#define motorStop 0x000


#define LEDLatchOn  0x010
#define LEDLatchOff 0x0EF
#define LEDMask   0x00F

#define Lift1Address (0x4020)

// Initialise functions
void Initialise(void);
void BusRead(void);
void BusWrite(void);


/* Initialisation */
void liftInit(void);

/* Functions to control the lift movement */ 
enum bool lift1Up (void);
enum bool lift1Down (void);
enum bool lift1Stop (void);

/* Function to check if a button has been pushed.
Returns true if a button has been pushed and with the button ID
otherwise returns false.
*/
enum bool buttonGet (uc_8 *buttonID);

/* Function to check if the cage is at a floor
Returns true if at a floor and with the floor ID
otherwise returns false.
*/
enum bool floorGet (uc_8 *floorID);

/* Function to switch a specific LED on or off 
LEDSelect must be in the range 0x00 to 0x0F (or 0 to 15 decimal) 
LEDState can take two values LEDOn or LEDOff 
Always returns true 
*/

#define LEDOn     0x020
#define LEDOff    0x000

enum bool WriteLed1(uc_8 LEDSelect, uc_8 LEDState);

// Open and close lift door with delays built in
void OpenDoor1 (void);
void CloseDoor1 (void);

/* Time delay function */
// Software time delay for simplicity here.
// Assume one cycle of the loop takes approximately 0.1microsecs
void Delay1();
void Delay2(int count);

#endif
