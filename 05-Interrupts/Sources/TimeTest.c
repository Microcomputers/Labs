/* Timetest.c
* Timetest Functions in c
* C.A.Maynard 20110113 v1.01
* Improved coding
* C.A.Maynard 20110201 v1.02
* Modified to work with the MSP430
* C.A.Maynard 20140703 v3.01
*/
#include "clic3.h"
#include "TimeTest.h"

/* configure the timer here*/
void TimeTestInit(void)
{
	//continuous mode!!!!
	TB0CTL = MC_2 | ID_0 | TASSEL_2;
}

#define TimeTestOffset (22)

void TimeTestStart(TimeTest *fred)
{
	fred->oldtcnt = TB0R;
}

void TimeTestEnd(TimeTest *fred)
{	
	ui_16 newtcnt;
 	//If called before TimeTestStart leave now!!!
 	// If the tcnt value is zero and no previous count get out!
	if (!((fred->oldtcnt == 0)&&(fred->count == 0))) 
	{	
		newtcnt = TB0R;
		fred->count += 1; // increment count
		newtcnt -= (fred->oldtcnt + TimeTestOffset);
		fred->totaltime += newtcnt;
		if (fred->count == 1) 
		{ // If first time through
			fred->min = newtcnt;
			fred->max = newtcnt;
			fred->mincount = 1;
			fred->maxcount = 1;
		}
		else if( newtcnt < fred->min) 
		{ // New minimum
			fred->min = newtcnt;
			fred->mincount = 1;
		}
		else if( newtcnt == fred->min )
		{
			fred->mincount++;
		} //another at minimum
		else if( newtcnt > fred->max) 
		{ // New maximum
			fred->max = newtcnt;
			fred->maxcount = 1;
		}
		else if( newtcnt == fred->max ) 
		{
				fred->maxcount++; 
		} // another at maximum
	    else {}
	}
}
