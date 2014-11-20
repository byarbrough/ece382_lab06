#include <msp430.h> 
#include "head06.h"



/*
 * main.c
 */
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    initMSP430();				//initialize system

}

void initMSP430(){

}

void drive(){

}
