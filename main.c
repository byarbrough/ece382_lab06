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

/*
 * TACCR1 is the duty cycle %.
 * It must be < 60 to prevent burning the motor chip
 */
void initMSP430(){
	IFG1=0; 					// clear interrupt flag1
	WDTCTL=WDTPW+WDTHOLD; 		// stop WD

	BCSCTL1 = CALBC1_8MHZ;		//calibrate clock
	DCOCTL = CALDCO_8MHZ;

	//set as output pins
	P1DIR |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5;

	GO_STOP;					//ensure motors are stopped
	EN_MOTOR_L;
	EN_MOTOR_R;					//enable both motors

	//Timer A
	P1DIR |= BIT6;                // TA0CCR1 on P1.6
	P1SEL |= BIT6;                // TA0CCR1 on P1.6

	TACTL &= ~MC1|MC0;            // stop timer A0

	TACTL |= TACLR;                // clear timer A0

	TACTL |= TASSEL1;           // configure for SMCLK

	TACCR0 = 100;                // set signal period to 100 clock cycles (~100 microseconds)
	TACCR1 = 25;                // set duty cycle to 25/100 (25%)

	TACCTL1 |= OUTMOD_7;        // set TACCTL1 to Reset / Set mode

	TACTL |= MC0;                // count up


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
