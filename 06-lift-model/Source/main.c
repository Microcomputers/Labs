//#include <msp430fr5739.h>
#include "lift1lib.h"
#include "clic3.h"

extern ui_16 BusAddress, BusData;



//uc_8 buttonStore[10] = {0,0,0,0,0,0,0,0,0,0};
uc_8 buttonQueue[20];

int main()
{
        ui_32 timer = 0;
        ui_32 second = 10000000 / 12;
        ui_32 timeout = 5 * second;
	uc_8 buttonstat, old_lvl, current_lvl, floorID;
	int moveUp = 0, moveDown = 0;
	//1)Initialising the lift system
	//2)Moving the lift up, down and stopping it.
	//lift1Down();
	//Delay1(3e6);
	//lift1Stop();
	//3)Detecting a button press and returning the button identity to the calling routine
	//4)Detecting a level in the shaft and returning the level identity to the calling routine
	//5)Switching on or off a specified LED associated with buttons etc
	//6)Delaying an approximate time period
	//7)Opening and closing the lift doors (simulated by time delay operations)
	Initialise();
	liftInit();
	turnLEDsON();
	turnLEDsOFF();
	moveInitiPos();	//move lift cage to initial pos
	//Determine to move up or down first time round
	//wait till a button is pressed
	while(!buttonGet(&buttonstat)){}
	if (buttonGet(&buttonstat))
	{
		processButton(buttonstat);
		if (buttonstat > level2ID)
		{
			moveUp   = 1;
			moveDown = 0;
		}
		if (buttonstat < level2ID)
		{
			moveUp   = 0;
			moveDown = 1;
		}	
	}
	while (1)
	{
		timer++;
		//uc_8 level;
		//int c = 0;
		//floorGet(&floorID);

		if(buttonGet(&buttonstat))
		{
			timer = 0; //reset timer counter
			processButton(buttonstat);
		/*
		buttonStore[0] = buttonstat;
		WriteLed1 (buttonStore[0], LEDon); 
		while(floorID != buttonStore[0])
		{
		if(buttonStore[0] > floorID)
		{
		lift1Up();
		while(floorGet(&floorID)==false){}
		}

		if(buttonStore[0] < floorID)
		{
		lift1Down();
		while(floorGet(&floorID)==false){}
		}
		}
		lift1Stop();
		WriteLed1 (buttonStore[0], LEDoff); 
		*/
		}
		else 
		{
			if (timer >= timeout)
			{
				moveInitiPos();
				timer = 0;
			}
		}
		if (moveUp)
		{
			lift1Up();
			while(floorGet(&floorID)==false){}
			liftbreak(floorID);
		}
		else if (moveDown)
		{
			lift1Down();
			while(floorGet(&floorID)==false){}
			liftbreak(floorID);    
		}
                BusAddress = Lift1Address;
                BusRead();
                floorID = (((BusData >> 1) & floorMask & 0x06));
		if (floorID == bottom)
		{
			moveUp   = 1;
			moveDown = 0;
		}
		else if (floorID == top)
		{
			moveUp   = 0;
			moveDown = 1;
		}
	}
}
    
      
      
      
      
      
      
	/*
	if(floorGet(&current_lvl))
	{
		
	}
	*/
		/*if(buttonGet(&buttonstat))
		{
			WriteLed1(buttonstat, LEDon);
			if (buttonstat%2==0)
			{
				lift1Up();
				//Delay1(25000000);
				//lift1Stop();

				while(!floorGet(&current_lvl)){}
				lift1Stop();
			}
			else
			{
				lift1Down();
				//Delay1(25000000);
				//lift1Stop();
				while(!floorGet(&current_lvl)){}
				lift1Stop();
			}
		}*/
	//Delay1(25000000);
	//WriteLed1(buttonstat, LEDoff);
      
      //WriteLed1(7, LEDon);
    


/////////////////////////////////////////////////////

/*void  Delay1(ui_32 DelayConst1){
	ui_32 i1;
	for (i1=0;i1<DelayConst1;i1++){}
}
*/
