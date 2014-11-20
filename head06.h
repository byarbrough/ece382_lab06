/*
 * head06.h
 *
 *  Created on: Nov 20, 2014
 *      Author: C16Brian.Yarbrough
 */

typedef		unsigned char		int8;
typedef		unsigned short		int16;
typedef		unsigned long		int32;
typedef		unsigned long long	int64;

//different motor commands for the robot
typedef enum	{FORWARD, BACKWARD, LEFT, RIGHT, STOP} direction;
#define		GO_FORWARD
#define		GO_BACKWARD
#define		GO_LEFT
#define		GO_RIGHT
#define		GO_STOP

#define		TRUE				1
#define		FALSE				0

void initMSP430();
__interrupt void pinChange (void);
__interrupt void timerOverflow (void);

#define		IR_PIN			(P2IN & BIT6)
#define		HIGH_2_LOW		P2IES |= BIT6
#define		LOW_2_HIGH		P2IES &= ~BIT6

//buttons on the Hitachi remote are commented out.
//buttons for my phone remote are active
#define		UP		0x20DF02FD //0x0AF50EF1
#define		DOWN	0x20DF827D //0x0AF58E71
#define		LEFT	0x20DFE01F //0x0AF5CE31
#define		RIGHT	0x20DF609F //0x0AF54EB1

#define		CH_UP	0x20DF718E //0x0AF59867
#define		CH_DW	0x20DFF10E //0x0AF518E7
#define		MUTE	0x20DF8D72 //0x0AF5D02F
#define		EXIT	0x20DFDA25 //0x0AF508F7

void drive();


#ifndef HEAD06_H_
#define HEAD06_H_





#endif /* HEAD06_H_ */
