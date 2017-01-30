#include "elev.h"
#include "fsm.h"
#include "timer.h"

#include <stdio.h>
#include <unistd.h>


int last_stop_button;
int last_floor_visited = -9999;

int main() {
    if (!elev_init()) {
        return 1;
    }
    fsm_initialize();


    while (1) {
        /* FLOOR SENSOR */
        int floor = elev_get_floor_sensor_signal();
        if(floor != last_floor_visited  &&  floor != -1){
            fsm_event_arrived_at_floor(floor);
        }
        last_floor_visited = floor;
        
        /* STOP BUTTON */
        int stopbtn = elev_get_stop_signal();
        if(stopbtn != last_stop_button){
            if(stopbtn){
                fsm_event_stop_pressed();
            } else {
                fsm_event_stop_released();
            }
        }
        last_stop_button = stopbtn;
        
        /*BUTTON PRESSED*/
        int buttonfloor;
        int buttontype;
        for (buttonfloor = 0; buttonfloor < N_FLOORS; buttonfloor++){
        	for(buttontype = 0; buttontype < N_BUTTONS; buttontype++){
            	int test = elev_get_button_signal(buttontype,buttonfloor);
            	if (test){
            		fsm_event_order_button_pressed(buttontype, buttonfloor);
					if (buttonfloor == last_floor_visited){
						fsm_event_arrived_at_floor(last_floor_visited);
					}
				}
        	}
        }
	
		/*DOOR CLOSED*/
		if (timer_isTimeOut()){
			fsm_event_door_closed();
		}
	}
    return 0;
}
