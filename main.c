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

    while(1){
    drive(FORWARD);
		__delay_cycles(1000000);
		GO_STOP;
		drive(LEFT_T);
		__delay_cycles(1000000);
		GO_STOP;
		drive(BACKWARD);
		__delay_cycles(1000000);
		GO_STOP;
		drive(RIGHT_T);
		__delay_cycles(1000000);
		GO_STOP;
    }

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
	P2DIR |= BIT3;                // TA0CCR1 on P2.3
	P2SEL |= BIT3;                // TA0CCR1 on P2.3
	P2IFG &= ~BIT3;				//clear interrupt flag
	P2IE |= BIT3;				//enable interrupt on bin change

	TACTL &= ~MC1|MC0;            // stop timer A0
	TACTL |= TACLR;                // clear timer A0
	TACTL |= TASSEL1;           // configure for SMCLK

	TACCR0 = 100;                // set signal period to 100 clock cycles
	TACCR1 = 25;                // set duty cycle to 25/100 (25%)

	TACCTL1 |= OUTMOD_3;        // set TACCTL1 to Set / Reset mode

	TACTL = ID_3 | TASSEL_2 | MC_1 | TAIE;

	_enable_interrupt();

}

void drive(direction movement){
	switch(movement){
	case	FORWARD:
		GO_FORWARD;
		break;
	case	BACKWARD:
		GO_BACKWARD;
		break;
	case	LEFT_T:
		GO_LEFT;
		break;
	case	RIGHT_T:
		GO_RIGHT;
		break;
	case	STOP:
		GO_STOP;
		break;
	}
}




