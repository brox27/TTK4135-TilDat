#include "timer.h"

#include <assert.h>
#include <stdbool.h>
#include <time.h>


static clock_t startTime = -1;
int duration;

void timer_start(){
    startTime = time(0);
    assert(startTime != -1);
	duration = 3;
}

int timer_isTimeOut(){
	if(startTime <0){
		return false;
	}
	
	time_t now = time(0);

    if(now - startTime > duration){
        return true;
  	}
    else{
        return false;
  	}
}

void timer_stop(){
	startTime = -1;
}
