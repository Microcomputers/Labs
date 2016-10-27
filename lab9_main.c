#include "clic3.h"
#include "lift1lib.h"

uc_8 temp,temp1;
int current_level = -1;

void delay(ui_32 timeout){
  Delay1(timeout * 10000);
}

void init_LEDs (uc_8 val, ui_32 millis) {
  for(uc_8 i =0x0 ; i  < 0xD; i++){
    WriteLed1(i,val);
    delay(millis);
  }
}

void led_fun_time(void){
  for(int i = 0; i < 5; i ++){
    init_LEDs(LEDOn,1);
    init_LEDs(LEDOff,1);
  }
}


int resolve_level(uc_8 l){
  int retval = -1;
  switch(l){
    case 0x07:
      retval = 0;
      break;
    case 0x00:
      retval = 1;
      break;
    case 0x01:
      retval = 2;
      break;
    case 0x02:
      retval = 3;
      break;
    case 0x03:
      retval = 4;
      break;
    case 0x04:
      retval = 5;
      break;
    case 0x08: // L1 +
      retval = 2;
      break;
    case 0x09: // L2 -
      retval = 1;
      break;
    case 0x0A: // L2 +
      retval = 3;
      break;
    case 0x0B: // L3 -
      retval = 2;
      break;
    case 0x0C: // L3 +
      retval = 4;
      break;
    case 0x0D: // L4 -
      retval = 3;
      break;
  }
  return retval;
}

void up(void){
  lift1Up();
  current_level = -1; //we don't know the level
  while(floorGet(&temp1)==false);
  current_level = resolve_level(temp1); //now we're at a level, find out what it is
  lift1Stop();
}

void down(void){
  lift1Down();
  current_level = -1; //we don't know the level
  while(floorGet(&temp1)==false);
  current_level = resolve_level(temp1) - 1;//now we're at a level, find out what it is
  lift1Stop();
}

int find_level(void){
  if (current_level == -1){ //check if we know the level already
    up(); //we're somewhere inbetween, go up
  }
  
  return current_level;
}

void goto_floor(int target_level){
  while(target_level < find_level()){
    down();
  }
  while(target_level > find_level()){
    up();
  }
}

void init_lift(){
  goto_floor(2);
}

void main(void) {
    enum bool flag;
    enum bool flag1,flag2;
    
    Initialise();
    liftInit();
    
    led_fun_time();
    init_lift();
    
    ui_32 second = 10000000 / 12;
    ui_32 timer = 0;
    ui_32 timeout = 5 * second;
    
    for(;;){
      timer ++;
      flag1 = buttonGet(&temp);
      if(flag1)
      {
        timer = 0;
        switch(temp){
          case 0x0A:
            up();
            break;
          case 0x09:
            down();
            break;
          default:
            int btn = resolve_level(temp);
            sevenSegPut(0, btn);
            goto_floor(btn);
            break;
        }

        
        flag1=false;
        temp=0;
      }
      else{
        if(timer >= timeout){
            timer = 0;
            goto_floor(2);
        }
      }
      
    }
}
