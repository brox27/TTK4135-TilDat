#include "elev.h"
#include "queue.h"

#include <stdbool.h>
#include <stdio.h>


int queue_button_already_in_queue(int buttontype, int buttonfloor);
int queue_orders_below(int floor);
int queue_orders_above(int floor);
int orders[4][3];

void queue_remove_all_orders(){
    int floor;
    int button;
    for (floor = 0; floor < N_FLOORS; floor++){
        for (button = 0; button < N_BUTTONS; button++){
            orders[floor][button]=0;
    	}
	}
}

void queue_clear_orders_at_floor (int floor){
    int button;
    for (button = 0; button < N_BUTTONS; button++){
        orders[floor][button]=0;
	}
}

int queue_button_already_in_queue(button, floor){
    if (orders[floor][button] == 1){
        return true;
	}
    return false;
}

int queue_orders_below(int floor){
    for (int f = (floor-1); f > -1; f--){
		for (int button = 0; button < N_BUTTONS; button++){
			if (orders[f][button]){
				return true;
			}
		}
	}
	return false;
}
  
int queue_orders_above(int floor){
    for (int f = (floor+1); f < N_FLOORS; f++){
		for (int button = 0; button < N_BUTTONS; button++){
			if (orders[f][button]){
				return true;
			}
		}
	}
	return false;
}


int queue_should_stop(int floor,int direction){
    if (orders[floor][BUTTON_COMMAND] == 1){
		return true;
    }
    if (direction == DIRN_UP){
        if (orders[floor][BUTTON_CALL_UP] == 1){
            return true;
		}
        if (queue_orders_above(floor))
            return false;
    }
    if (direction == DIRN_DOWN){
        if (orders[floor][BUTTON_CALL_DOWN] == 1){
            return true;
		}
        if (queue_orders_below(floor)){
            return false;
		}
    }

    return true;
}

int queue_get_direction(int previous_direction, int floor){
	if (previous_direction == DIRN_UP){
		if (queue_orders_above(floor)){
		    return DIRN_UP;
		}
	}
	if (previous_direction == DIRN_DOWN){
		if (queue_orders_below(floor)){
			return DIRN_DOWN;
		}
	}

	if (queue_orders_above(floor)){
		return DIRN_UP;
		}
	if (queue_orders_below(floor)){

		    return DIRN_DOWN;
		    }

	return DIRN_STOP;
}

void queue_add_orders(int button,int floor){
	if (!queue_button_already_in_queue(button, floor)){
		orders[floor][button] = 1;
	}
}
