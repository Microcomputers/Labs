//#include <msp430fr5739.h>
#include "lift1lib.h"



int main()
{
    uc_8 button_current, button_previous;
    uc_8 level_old;
    uc_8 level_current;
    uc_8 floorID;
    uc_8 button_store[MAX_BUTTONS];
    
    level_current=0;
    
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
   
    turnLEDsON();
    Delay1(2500);
    turnLEDsOFF();
    moveInitiPos();	//move lift cage to initial pos
    //Delay1(2500);
    level_current = level2ID;
    while (1)
    {
      
      //  uc_8 level;
      //  int c = 0;
      //  floorGet(&level_current);
        if (floorGet(&level_current))
        {

        	buttonProcess();
			while(level_current != button_store[0])
			{
				if(button_store[0] > level_current)
				{
					lift1Up();
					while(!floorGet(&level_current)){}
					
				}
				if(button_store[0] < level_current)
				{
					lift1Down();
					while(!floorGet(&level_current)){}
					
				}
			}
			lift1Stop();
                        WriteLed1(button_store[0], LEDoff);
			buttonMoveIndex();
        }
        else
        {
			buttonProcess();
                        

        }
    }    
}
      
      
      
      
      
      
      
	/*
	if(floorGet(&level_current))
	{
		
	}
	*/
		/*if(buttonGet(&button_current))
		{
			WriteLed1(button_current, LEDon);
			if (button_current%2==0)
			{
				lift1Up();
				//Delay1(25000000);
				//lift1Stop();

				while(!floorGet(&level_current)){}
				lift1Stop();
			}
			else
			{
				lift1Down();
				//Delay1(25000000);
				//lift1Stop();
				while(!floorGet(&level_current)){}
				lift1Stop();
			}
		}*/
	//Delay1(25000000);
	//WriteLed1(button_current, LEDoff);
      
      //WriteLed1(7, LEDon);
    


