//#include <msp430fr5739.h>
#include "lift1lib.h"

/* Time delay function */
// Software time delay for simplicity here.
// Assume one cycle of the loop takes approximately 0.1microsecs
void Delay2(int count);
//void Delay1(ui_32 DelayConst1);
void Delay();

//This two functions test LEDs
// Turn LEDs ON, on start up
void turnLEDsON();
// Turn LEDs OFF, on start up
void turnLEDsOFF();
long long int cc;

int main()
{
    uc_8 buttonstat, old_lvl, current_lvl, floorID;
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
  //  Delay1(2500);
   turnLEDsOFF();
    moveInitiPos();	//move lift cage to initial pos
    //Delay1(2500);
    cc = 0;
    while (1)
    {
      
        uc_8 level;
        int c = 0;
        
      
      floorGet(&floorID);
      
        if(buttonGet(&buttonstat))
        {
          while(floorID != buttonstat)
          {
            if(buttonstat > floorID)
            {
              lift1Up();
              while(floorGet(&floorID)==false){}
            }
            
              if(buttonstat < floorID)
            {
              lift1Down();
              while(floorGet(&floorID)==false){}
            }
            
          }
          lift1Stop();
        
           }
         
        else {
          cc++;
          if (cc >= 2500000)
          {
            cc=0;
            moveInitiPos();
          }
          
          
            
            
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
//100 ms delay
void  Delay()
{
	ui_32 DelayConst1 = 1000000;
	ui_32 i1;
	for (i1=0;i1<DelayConst1;i1++){}
}


void Delay2(int count)
{
	for (int j = 0; j < count; ++j)
	{
		Delay();
	}
}

//int LEDAddress[] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd};

void turnLEDsON()
{
	for (int i = 0; i < 12; ++i)
	{
		WriteLed1(i, LEDon);
                Delay2(2);
	}
}

void turnLEDsOFF()
{
	for (int i = 12; i >0; i--)
	{
		WriteLed1(i, LEDoff);
                Delay2(2);
	}
}