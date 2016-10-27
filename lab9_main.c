#include "clic3.h"
#include "lift1lib.h"

#define FLOORCOUNT 4

#define DONTSTOP -1 // equivilient to no buttons
#define STOPUP 0 // equivilient to the up button
#define STOPDOWN 1 // equivilient to the down button
#define STOPALWAYS 2 // equivilient to the main button

//Struct definition for a floor
typedef struct {
        int  stop; // contains the stop conditions for the floor, eg: DONTSTOP, STOPUP, STOPDOWN, STOPALWAYS
	uc_8 btns[3]; // contains the HEX for the buttons and lights asscioated with this floor. The index for this array should match the stop contditions eg: btns[STOPUP] = up button/LED
}Floor;

Floor floors[FLOORCOUNT + 1] = {
	{ //dummy floor to take position zero
		.stop = DONTSTOP,
		.btns = {
			0xFF, //up
			0xFF, //down
			0xFF  //main
		}
	},
	{ //Floor ONE
		.stop = DONTSTOP,
		.btns = {
			0x08, //up
			0xFF, //down
			0x00  //main
		}
	},
	{ //Floor TWO
		.stop = DONTSTOP,
		.btns = {
			0x0A, //up
			0x09, //down
			0x01  //main
		}
	},
	{ //Floor THREE
		.stop = DONTSTOP,
		.btns = {
			0x0C, //up
			0x0B, //down
			0x02  //main
		}
	},
	{ //Floor FOUR
		.stop = DONTSTOP,
		.btns = {
			0xFF, //up
			0x0D, //down
			0x03  //main
		}
	}
};

int current_floor = -1;

void set_all_LEDS(uc_8 val, ui_32 millis) {
	for (uc_8 i = 0x0; i < 0xD; i++) {
		WriteLed1(i, val);
		if (millis > 0) {
			Delay1(10000);
		}
	}
}

void led_fun_time(void) {
	for (int i = 0; i < 5; i++) {
		set_all_LEDS(LEDOn, 1);
		set_all_LEDS(LEDOff, 1);
	}
}

//get a human readable integer for the floor number
//these intagers start at 1 not 0 to make life easier
int resolve_floor(uc_8 l) {
	int retval = (int)l;
	retval = retval + 1;
        if (retval > 4){
          retval = 4;
        }
        if (retval < 1){
          retval = 1;
        }
        return retval;
}

//implement all the functionality required when the left stops at floor
enum bool stop_at_floor(int floor) {
      floors[floor].stop = DONTSTOP;
      
      WriteLed1(floors[floor].btns[STOPUP], LEDOff);
      WriteLed1(floors[floor].btns[STOPDOWN], LEDOff);
      WriteLed1(floors[floor].btns[STOPALWAYS], LEDOff);	
      
      Delay1(10000000);
}

//check if we need to stop at the current floor based on the direction
enum bool check_floor_stops(int floor, int direction) {
        enum bool retval = false;
	if (floors[floor].stop != DONTSTOP) {
		if (floors[floor].stop == STOPALWAYS) {
			retval = true;
		}
		if (floors[floor].stop == STOPUP && direction > 0) {
			retval = true;
		}
		if (floors[floor].stop == STOPDOWN && direction < 0) {
			retval = true;
		}
	}
	return retval;
}

//Find a new objective (destination floor)
int get_objective(int floor, int dir) {
	for (int i = floor; (i > 0 && i < 5); i = i + dir) {
		if (floors[i].stop != DONTSTOP) {
			return i;
		}
	}
	for (int i = floor; (i > 0 && i < 5); i = i - dir) {
		if (floors[i].stop != DONTSTOP) {
			return i;
		}
	}
	return -1; // there is no objective
}

//Process a button that has been pressed:
//Turn on it's LED
//Tag the relevant floor with the stop conditions
void button_pressed(uc_8 btn_pressed) {
	//turn on that led
	WriteLed1(btn_pressed, LEDOn);
	//find the relevant floor/button
	for (int i = 1; i < FLOORCOUNT + 1; i++) {
		for (int btn = 0; btn < 3; btn++) {
			//now work out the new stop condition for the floor;
			if (floors[i].btns[btn] == btn_pressed) {
				if (floors[i].stop == DONTSTOP) {
					floors[i].stop = btn;
				}
				else {
					if (floors[i].stop != btn) {
						floors[i].stop = STOPALWAYS;
					}
				}
			}
		}
	}
}

//This "tick" should be happening very often as it listens for the user input
//This should be called from any wait loops.
uc_8 btn;
enum bool wait_tick() {
	enum bool flag1;        
	flag1 = buttonGet(&btn);
	if (flag1)
	{
		button_pressed(btn);
	}
	return flag1; // return true if something interesting happened
}

//for some reason the read floor function returns the wrong value on the first read.
//This function implements the wait loop (calling the wait_tick in side it) and then attempts to read the floor a second time
//What I've found is if the second read is successful, then the first one was wrong
uc_8 move_wait() {
	uc_8 temp3, temp4;
	while (floorGet(&temp3) == false) {
		wait_tick();
	};
	//this second check fixes a bug with the flor check code
	if (floorGet(&temp4)) { temp3 = temp4; }
        return temp3;
}

//Move the lift one floor in a given direction
// +1 = move up
// -1 = move down
int move(int direction) {
	if (direction > 0) {
		lift1Up();
	}
	if (direction < 0) {
		lift1Down();
	}
	current_floor = -1; //now that the lift is moving it isn't on a floor
	uc_8 floor = move_wait();
        lift1Stop();
	current_floor = resolve_floor(floor); //now we're at a level, find out what it is
	if (check_floor_stops(current_floor, direction)) {
		stop_at_floor(current_floor);
	}	
	return current_floor;
}

// Get the current level
// If the lift is currently inbetween levels or it's unknown for some reason then move up until a floor is found
int find_level(void) {
	if (current_floor == -1) { //check if we know the level already
		move(+1); //we're somewhere inbetween, go up
	}

	return current_floor;
}

//Go to a given floor
void goto_floor(int target_level) {
	while (target_level < find_level()) {
		move(-1);
	}
	while (target_level > find_level()) {
		move(+1);
	}
}

//initialize the lift
void init_lift() {
	goto_floor(2);
}

void main(void) {
	enum bool flag;
	enum bool flag1, flag2;

	Initialise();
	liftInit();

	led_fun_time();
	init_lift();

	ui_32 second = 50000; // roughly equal to one second	
	ui_32 timeout = 5 * second;

        ui_32 timer = 0;
        
	for (;;) {
		timer++;
		flag2 = wait_tick();
                
		if (current_floor == -1) {
                    //for some reason we don't know the floor, move up til we find one
                    move(+1);
                }
                else{
			int objective = get_objective(current_floor, +1);

			//is there any floors we need to go to
			if (objective != -1)
			{
				timer = 0;
				goto_floor(objective);

				flag1 = false;
			}
			else {
				if (timer >= timeout) {
					timer = 0;
					goto_floor(2);
				}
			}
		}

	}
}
