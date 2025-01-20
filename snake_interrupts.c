//******************************************************************************
//	snake_interrupts.c
//
//  Author:			Paul Roper, Brigham Young University
//  Revisions:		1.0		11/25/2012	RBX430-1
//
//******************************************************************************
//
#include "msp430.h"
#include <stdlib.h>
#include "RBX430-1.h"
#include "RBX430_lcd.h"
#include "snake.h"

extern volatile uint16 WDT_cps_cnt;			// WDT count per second
extern volatile uint16 WDT_move_cnt;		// counter to adc read event
extern volatile uint16 WDT_debounce_cnt;
extern volatile uint32 WDT_delay;
extern volatile uint16 WDT_Tone_Cnt;

extern volatile uint16 sys_event;			// pending events

extern volatile uint16 move_cnt;			// snake speed
extern volatile uint8 game_mode;			// 0=idle, 1=game in progress
extern volatile uint8 level;				// current level (1-4)
extern volatile uint8 seconds;
//------------------------------------------------------------------------------
//-- Port 1 ISR ----------------------------------------------------------------
//
#pragma vector=PORT1_VECTOR
__interrupt void Port_1_ISR(void)
{
	if (P1IFG & 0x0f)
	{
		P1IFG &= ~0x0f;						// P1.0-3 IFG cleared
		WDT_debounce_cnt = DEBOUNCE_CNT;	// enable debounce
	}
	if (sys_event) __bic_SR_register_on_exit(LPM0_bits);
	return;
} // end Port_1_ISR


//-- Watchdog Timer ISR --------------------------------------------------------
//
#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR(void)
{
	if (--WDT_cps_cnt == 0)					// 1 second counter
	{
		LED_GREEN_TOGGLE;					// toggle GREEN led
		WDT_cps_cnt = WDT_CPS;
		sys_event |= LCD_UPDATE;
		seconds--;
		/*if (game_mode==PLAY) {
					LCD_UPDATE_event();
					seconds = seconds - 1;
					if (seconds == 0) {
						seconds = 30;
						check = 0;
						endGame = 1;
						END_GAME_event();
					}
				}*/
		/*
		 * if (game_mode==PLAY) {
			LCD_UPDATE_event();
			timer = timer - 1;
			if (timer == 0) {
				sys_event = END_GAME;
			}
		}
		WDT_cps_cnt = WDT_CPS;
		 *
		 */


//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//	Add code here for 1 second events
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	}

	if (WDT_Tone_Cnt && (--WDT_Tone_Cnt == 0))
   {
	  TBCCR0 = 0;
   }

	if (WDT_delay && (--WDT_delay == 0));

	if ((game_mode == PLAY) && (--WDT_move_cnt == 0))	// update move counter
	{
		WDT_move_cnt = move_cnt;
		sys_event |= MOVE_SNAKE;			// MOVE SNAKE event
	}

	// check for switch debounce
	if (WDT_debounce_cnt && (--WDT_debounce_cnt == 0))
	{
		sys_event |= (P1IN ^ 0x0f) & 0x0f;
	}

	if (sys_event) __bic_SR_register_on_exit(LPM0_bits);
	return;
} // end WDT_ISR


//------------------------------------------------------------------------------
//--MSP430 INTERRUPT VECTORS----------------------------------------------------

#pragma vector=PORT2_VECTOR
__interrupt void Port_2_ISR(void)
{
	ERROR2(11);
}

#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
	ERROR2(12);
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void)
{
	ERROR2(13);
}

#pragma vector=TIMERA1_VECTOR
__interrupt void TIMERA1_ISR(void)
{
	ERROR2(14);
}

#pragma vector=TIMERA0_VECTOR
__interrupt void TIMERA0_ISR(void)
{
	ERROR2(15);
}

#pragma vector=TIMERB1_VECTOR
__interrupt void TIMERB1_ISR(void)
{
	ERROR2(16);
}

#pragma vector=TIMERB0_VECTOR
__interrupt void TIMERB0_ISR(void)
{
	ERROR2(17);
}

#pragma vector=NMI_VECTOR
__interrupt void NMI_ISR(void)
{
	ERROR2(18);
}
