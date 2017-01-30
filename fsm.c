#include "elev.h"
#include "fsm.h"
#include "queue.h"
#include "timer.h"

#include <stdio.h>


typedef enum{
	INITIALIZE,
	IDLE,
	MOVING,
	DOOROPEN,
	EMERGENCYSTOP,
	STOPBETWEENFLOORS
} state_t;

void fsm_clear_all_lights();

state_t currentstate = INITIALIZE;
int     currentfloor;
int     direction;
int     previous_direction; 
int     previous_floor; 
int		button;
int		floorbtn;

void fsm_initialize(){  
	
	elev_set_motor_direction(DIRN_DOWN);	
}

void fsm_clear_all_lights(){
        for(floorbtn = 0; floorbtn < N_FLOORS; floorbtn++){
            for(button = 0; button < N_BUTTONS; button++){
                elev_set_button_lamp(button,floorbtn,0);
            }
        }
}

void fsm_event_arrived_at_floor(int new_floor){
	currentfloor = new_floor;
	switch(currentstate){
	case INITIALIZE:
		elev_set_motor_direction(DIRN_STOP);
		elev_set_floor_indicator(currentfloor);
		currentstate = IDLE;
		break;

	case IDLE:
	case DOOROPEN:
			elev_set_door_open_lamp(1);
			queue_clear_orders_at_floor(currentfloor);
			timer_start();

			for(button = 0; button < N_BUTTONS; button++){
                	elev_set_button_lamp(button,currentfloor,0);
	        }
		break;

	case MOVING:
		elev_set_floor_indicator(currentfloor);

		if(queue_should_stop(currentfloor, direction)){
			elev_set_motor_direction(DIRN_STOP);
			elev_set_door_open_lamp(1);
			queue_clear_orders_at_floor(currentfloor);
			timer_start();
			currentstate = DOOROPEN;

			for(button = 0; button < N_BUTTONS; button++){
                	elev_set_button_lamp(button,currentfloor,0);
	        	}
		
		}
		break;

	default:
		break;
	}
}

void fsm_event_door_closed(){
	switch(currentstate){
	case DOOROPEN:
		elev_set_door_open_lamp(0);
		direction = queue_get_direction(previous_direction, currentfloor);
		elev_set_motor_direction(direction);
		previous_direction = direction;

		if(direction!=DIRN_STOP){
			currentstate = MOVING;
		} 
		else {
			currentstate = IDLE;
		}		
		break;

	default:
		break;
	}		
}

void fsm_event_stop_pressed(){ 
	switch(currentstate){
	case MOVING:
	case DOOROPEN:
	case STOPBETWEENFLOORS:
	case IDLE:
		elev_set_motor_direction(DIRN_STOP);
		queue_remove_all_orders();
		fsm_clear_all_lights();
		elev_set_stop_lamp(1);

		if (elev_get_floor_sensor_signal() != -1){
			elev_set_door_open_lamp(1);
			timer_start();
		}

		currentstate = EMERGENCYSTOP;
		break;

	default:
		break;
	}
}

void fsm_event_stop_released(){
	switch(currentstate){
	case EMERGENCYSTOP:
		elev_set_stop_lamp(0);
		
		if (elev_get_floor_sensor_signal() == -1){
		    currentstate = STOPBETWEENFLOORS;
		}
		else {
			elev_set_door_open_lamp(0);
			currentstate = IDLE;
			
		}
		break;

	default:
		break;
	}
}
    
void fsm_event_order_button_pressed(button, floor){ 
	switch(currentstate){
        case IDLE:

        	if (floor!=currentfloor){
				queue_add_orders(button, floor);
				elev_set_button_lamp(button, floor, 1);
				direction = queue_get_direction(previous_direction, currentfloor);
				elev_set_motor_direction(direction);
				previous_direction = direction;
				currentstate = MOVING;
			}
			else{
				elev_set_door_open_lamp(1);
				timer_start();
				currentstate = DOOROPEN;
			
			} 
		case DOOROPEN:
		case MOVING:
			queue_add_orders(button, floor);
		    	elev_set_button_lamp(button, floor, 1);

			break;

		case STOPBETWEENFLOORS:
			queue_add_orders(button, floor);

			if (floor==currentfloor){
				elev_set_motor_direction(-(previous_direction));
			} 
			else {
				direction = queue_get_direction(previous_direction, currentfloor);
				elev_set_motor_direction(direction);
			}
			currentstate = MOVING;
			break;

	default:
        	break;
	}
}


	

	
	
