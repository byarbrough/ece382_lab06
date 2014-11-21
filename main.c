//-----------------------------------------------------------------
// Name:	Brian Yarbrough
// Class:	ECE 382 Section T5, Capt Trimble
// Start:	Coulston
// File:	lab5.c
// Date:	Fall 2014
// Purp:	Implement LCD Etch-a-Sketch with the IR remote
//-----------------------------------------------------------------

#include <msp430.h> 
#include "head06.h"



/*
 * main.c
 */
void main(void) {
	
    initMSP430();				//initialize system

}

void initMSP430(){
	IFG1=0; 					// clear interrupt flag1
	WDTCTL=WDTPW+WDTHOLD; 		// stop WD

	BCSCTL1 = CALBC1_8MHZ;		//calibrate clock
	DCOCTL = CALDCO_8MHZ;

	//set as output pins
	P2DIR |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5

	P2OUT &= GO_STOP					//ensure motors are stopped
	P2OUT |= EN_MOTOR_L | EN_MOTOR_R	//endable both motors

}

void drive(direction movement, int16 duration){
	switch(movement){
	case	FORWARD:
		break;
	case	BACKWARD:
		break;
	case	LEFT_T:
		break;
	case	RIGHT_T:
		break;
	case	STOP:
		break;
	}
}
