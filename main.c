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

int16	packetData[34];
int8	packetIndex = 0;

/*
 * main.c
 */
void main(void) {
	
    initMSP430();				//initialize system
    initIR();

    P1DIR |= BIT0 | BIT6;				// Enable updates to the LED
   	P1OUT &= ~(BIT0 | BIT6);			// An turn the LED off

    GO_STOP;

    while(1){
    	if(packetIndex == 34){
    		handlePress();
    		_delay_cycles(LONG_T);
    		initIR();
    	}
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

    P1DIR &= ~BIT3;
    P1REN |= BIT3;

    P2DIR |= BIT2;							// P2.2 is associated with TA1CCR1
    P2SEL |= BIT2;							// P2.2 is associated with TA1CCTL1

    P2DIR |= BIT4;							// P2.4 is associated with TA1CCR2
    P2SEL |= BIT4;							// P2.4 is associated with TA1CCTL2

    //ports for direction
    P2DIR |= BIT0 | BIT1 | BIT3 | BIT5;

	TA1CTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:8 presclar off MCLK
    TA1CCR0 = 1000;						// set signal period

    TA1CCR1 = 500;
    TA1CCTL1 = OUTMOD_3;					// set TACCTL1 to Reset / Set mode

    TA1CCR2 = 500;
    TA1CCTL2 = OUTMOD_3;					// set TACCTL1 to Reset / Set mode
}

void initIR(){
    //IR Pins
    P2SEL  &= ~BIT6;						// Setup P2.6 as GPIO not XIN
    P2SEL2 &= ~BIT6;
    P2DIR &= ~BIT6;
    P2IFG &= ~BIT6;						// Clear any interrupt flag
    P2IE  |= BIT6;						// Enable PORT 2 interrupt on pin change
    TACTL &= ~TAIFG;					// clear flag before enabling interrupts = good practice
    TACTL = ID_3 | TASSEL_2 | MC_1 | TAIE;		// Use 1:1 presclar off MCLK and enable interrupts

    _enable_interrupt();
}


void handlePress(){
	_disable_interrupt();
	int32 result = 0;
	int32 setter = 0x80000000;				//1 in the MSB

	char i;
	for(i = 2; i<34; i++){					//traverse array
		int16 current = packetData[i];		//current element
		packetData[0] = 0;					//clear element

		if(current/10 < 100){				//is a zero
			result &= ~setter;				//clear bit
		}
		else {
			result |= setter;				//set bit
		}
		setter >>= 1;						//rotate setter
		}

	packetIndex++;							//ensure the loop is not reentered

	switch(result){							//take appropriate action

	case UP:
		P1OUT ^= BIT0;
		drive(FORWARD);
		break;

	case DOWN:
		P1OUT ^= BIT6;
		drive(BACKWARD);
		break;

	case LEFT:
		P1OUT |= (BIT0 | BIT6);
		drive(LEFT_T);
		break;

	case RIGHT:
		P1OUT ^= (BIT0 | BIT6);
		drive(RIGHT_T);
		break;

	case EXIT:
		drive(STOP);
		break;

	case CH_UP:

		break;

	case CH_DW:

		break;

	case MUTE:
		break;
	}

	initIR();

}

void drive(direction movement){
	GO_STOP;
	_delay_cycles(SHORT_T);

	switch(movement){
	case	FORWARD:
		TA1CCTL1 = OUTMOD_7;
		TA1CCTL2 = OUTMOD_7;
		GO_FORWARD;
		break;

	case	BACKWARD:
		TA1CCTL1 = OUTMOD_3;
		TA1CCTL2 = OUTMOD_3;
		GO_BACKWARD;
		break;

	case	LEFT_T:
		TA1CCTL1 = OUTMOD_7;
		TA1CCTL2 = OUTMOD_3;
		GO_FORWARD;
		GO_LEFT;
		break;

	case	RIGHT_T:
		TA1CCTL1 = OUTMOD_3;
		TA1CCTL2 = OUTMOD_7;
		GO_FORWARD;
		GO_RIGHT;
		break;

	case	STOP:
		GO_STOP;
		break;
	}//end switch

	ENABLE_MOTORS;
}

#pragma vector = PORT2_VECTOR			// This is from the MSP430G2553.h file
__interrupt void pinChange (void) {

	int8	pin;
	int16	pulseDuration;			// The timer is 16-bits

	if (IR_PIN)		pin=1;	else pin=0;

	switch (pin) {					// read the current pin level
		case 0:						// !!!!!!!!!NEGATIVE EDGE!!!!!!!!!!
			pulseDuration = TAR;
			TACTL = 0;
			packetData[packetIndex++] = pulseDuration;
			LOW_2_HIGH; 				// Setup pin interrupr on positive edge
			break;

		case 1:							// !!!!!!!!POSITIVE EDGE!!!!!!!!!!!
			TAR = 0x0000;						// time measurements are based at time 0
			TA0CCR0 = 0xFF00;					// create a 16mS roll-over period
			TACTL &= ~TAIFG;					// clear flag before enabling interrupts = good practice
			TACTL = ID_3 | TASSEL_2 | MC_1 | TAIE;		// Use 1:1 presclar off MCLK and enable interrupts
			HIGH_2_LOW; 						// Setup pin interrupr on positive edge
			break;
	} // end switch


	P2IFG &= ~BIT6;			// Clear the interrupt flag to prevent immediate ISR re-entry

} // end pinChange ISR



// -----------------------------------------------------------------------
// This interrupt is a way of reseting the reciever when something gets messed up
// It also resets packetIndex after the final pulse is recieved
// It is only active while IRPIN is high
//
// -----------------------------------------------------------------------
#pragma vector = TIMER0_A1_VECTOR			// This is from the MSP430G2553.h file
__interrupt void timerOverflow (void) {

	initMSP430();

	packetIndex = 0;

	TACTL &= ~TAIFG;		//clear flag

}


