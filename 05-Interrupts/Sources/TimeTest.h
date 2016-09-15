/* TimeTest.h
 * Provides a time testing facility for code particularly tasks
 * C.A.Maynard 20021006 v2.01
 * Modified to provide ability to compute average execution time as well as getting max and min.
 * C.A.Maynard 20110111 v2.02
 * Modified to work with the MSP430 in CLIC3 board using TimerB functionality
 * C.A.Maynard 20140704 v3.01
 */
#ifndef timefuncs
#define timefuncs

 typedef struct {ui_16 oldtcnt; ui_16 count; ui_16 min; ui_16 mincount; ui_16 max; ui_16 maxcount; ui_32 totaltime;} TimeTest ;

/* Structure must be initialised as follows
 * { 0, 0, 65535, 0, 0, 0, 0}
 */
/* Prototypes */

/* Initialise the timing for the system
NOTE this needs to be independent of other timer uses so don't overlap
*/
void TimeTestInit(void);
 
/* TimeTestStart initialises each measurement */
void TimeTestStart(TimeTest *fred);

/* TimeTestEnd completes each measurement and leaves results in the structure
 * Can appear in the code before TimeTestStart to assist measuring looping situations

 * The structure values returned:
 * Last starting TCNT. Not of value after execution stopped
 * Provides a count of the number of code executions .count
 * The minimum time execution in cycles and count: .min and .mincount
 * The maximum time execution in cycles and count: .max and .maxcount
 * The total time of all measurements: .totaltime
 *
 * SO average time is this .totaltime/.count
 */
void TimeTestEnd(TimeTest *fred);

#endif
