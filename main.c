//-----------------------------------------------------------------
// Name:	Brian Yarbrough
// Class:	ECE 382 Section T5, Capt Trimble
// Start:	Coulston
// File:	lab5.c
// Date:	Fall 2014
// Purp:	Implement LCD Etch-a-Sketch with the IR remote
//-----------------------------------------------------------------

#include <msp430g2553.h>
#include "head06.h"



/*
 * main.c
 */
void main(void) {
	
    initMSP430();				//initialize system

    while(1){

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

	 P2DIR |= BIT2;							// P2.2 is associated with TA1CCR1
	P2SEL |= BIT2;							// P2.2 is associated with TA1CCTL1

	P2DIR |= BIT4;							// P2.4 is associated with TA1CCR2
	P2SEL |= BIT4;							// P2.4 is associated with TA1CCTL2

	TA1CTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:8 presclar off MCLK
	TA1CCR0 = 0x0100;						// set signal period

	TA1CCR1 = 0x0020;
	TA1CCTL1 = OUTMOD_7;					// set TACCTL1 to Reset / Set mode

	TA1CCR2 = 0x0020;
	TA1CCTL2 = OUTMOD_3;					// set TACCTL1 to Reset / Set mode


}

void drive(direction movement){
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
		GO_STOP;
		break;
	}
}




