//#include <msp430fr5739.h>
#include "lift1lib.h"



int main()
{
    uc_8 buttonstat, old_lvlstat, current_lvlstat;
	//1)Initialising the lift system
    Initialise();
	liftInit();
	//2)Moving the lift up, down and stopping it.
        //lift1Down();
        //Delay1(3e6);
        //lift1Stop();
	//3)Detecting a button press and returning the button identity to the calling routine
	//4)Detecting a level in the shaft and returning the level identity to the calling routine
	//5)Switching on or off a specified LED associated with buttons etc
	//6)Delaying an approximate time period
	//7)Opening and closing the lift doors (simulated by time delay operations)
    while (1)
    {
	/*
	if(floorGet(&current_lvlstat))
	{
		
	}
	*/
		if(buttonGet(&buttonstat))
		{
			WriteLed1(buttonstat, LEDon);
			if (buttonstat%2==0)
			{
				lift1Up();
				//Delay1(25000000);
				//lift1Stop();
				while(!floorGet(&current_lvlstat)){}
				lift1Stop();
			}
			else
			{
				lift1Down();
				//Delay1(25000000);
				//lift1Stop();
				while(!floorGet(&current_lvlstat)){}
				lift1Stop();
			}
		}
	//Delay1(25000000);
	WriteLed1(buttonstat, LEDoff);
    }
	return 0;
}
