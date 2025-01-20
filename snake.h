//------------------------------------------------------------------------------
// snake.h
//------------------------------------------------------------------------------
#ifndef SNAKE_H_
#define SNAKE_H_

//-- system constants ----------------------------------------------------------
#define myCLOCK		12000000				// clock speed
#define CLOCK		_12MHZ

//-- watchdog constants --------------------------------------------------------
#define WDT_CLK		32000				// 32 Khz WD clock (@1 Mhz)
#define	WDT_CTL		WDT_MDLY_32			// WDT SMCLK, ~32ms
#define	WDT_CPS		myCLOCK/WDT_CLK		// WD clocks / second count

#if WDT_CPS/50
#define DEBOUNCE_CNT	WDT_CPS/50		// 20 ms debounce count
#else
#define DEBOUNCE_CNT	1				// 20 ms debounce count
#endif

#define WDT_LCD		WDT_CPS/4			// 250 ms
#define WDT_MOVE1	WDT_CPS/4			// 250 ms
#define WDT_MOVE2	WDT_CPS/8			// 125 ms
#define WDT_MOVE3	WDT_CPS/16			// 62 ms
#define WDT_MOVE4	WDT_CPS/32			// 31 ms

//-- sys_events ----------------------------------------------------------------
#define SWITCH_1	0x0001
#define SWITCH_2	0x0002
#define SWITCH_3	0x0004
#define SWITCH_4	0x0008

#define START_LEVEL	0x0010
#define NEXT_LEVEL	0x0020
#define END_GAME	0x0040
#define NEW_GAME	0x0080

#define MOVE_SNAKE	0x0100
#define LCD_UPDATE	0x0200

#define TONE_C    myCLOCK/1308/12*10
#define TONE_Cs   myCLOCK/1386/12*10
#define TONE_D    myCLOCK/1468/12*10
#define TONE_Eb   myCLOCK/1556/12*10
#define TONE_E    myCLOCK/1648/12*10
#define TONE_F    myCLOCK/1746/12*10
#define TONE_Fs   CLOCK/1850/12*10
#define TONE_G    CLOCK/1950/12*10
#define TONE_Ab   CLOCK/2077/12*10
#define TONE_A    CLOCK/2200/12*10
#define TONE_Bb   CLOCK/2331/12*10
#define TONE_B    CLOCK/2469/12*10
#define TONE_C1   CLOCK/2616/12*10
#define TONE_Cs1  CLOCK/2772/12*10
#define TONE_D1   CLOCK/2937/12*10
#define TONE_Eb1  CLOCK/3111/12*10
#define TONE_E1   CLOCK/3296/12*10
#define TONE_F1   CLOCK/3492/12*10
#define TONE_Fs1  CLOCK/3700/12*10
#define TONE_G1   CLOCK/3920/12*10
#define TONE_Ab1  CLOCK/4150/12*10
#define TONE_A1   CLOCK/4400/12*10
#define TONE_Bb1  CLOCK/4662/12*10
#define TONE_B1   CLOCK/4939/12*10



//-- service routine events ----------------------------------------------------
void SWITCH_1_event(void);
void SWITCH_2_event(void);
void SWITCH_3_event(void);
void SWITCH_4_event(void);

void START_LEVEL_event(void);
void NEXT_LEVEL_event(void);
void END_GAME_event(void);
void NEW_GAME_event(void);

void MOVE_SNAKE_event(void);
void LCD_UPDATE_event(void);

//-- snake game equates --------------------------------------------------------
#define START_SCORE			0
#define X_MAX	24						// columns
#define Y_MAX	23						// rows
#define X_MIN	0						// columns
#define Y_MIN	0						// rows

#define MAX_SNAKE			128			// max snake length (make power of 2)
#define MAX_FOOD			10			// max # of foods

#define TIME_1_LIMIT		30
#define LEVEL_1_FOOD		MAX_FOOD	// 10

#define TIME_2_LIMIT		30
#define LEVEL_2_FOOD		MAX_FOOD

#define TIME_3_LIMIT		45
#define LEVEL_3_FOOD		MAX_FOOD

#define TIME_4_LIMIT		60
#define LEVEL_4_FOOD		MAX_FOOD

enum {RIGHT, UP, LEFT, DOWN};			// movement constants
enum modes {IDLE, PLAY, NEXT};			// player modes

typedef struct							// POINT struct
{
	uint8 x;
	uint8 y;
} POINT;
typedef struct food_struct
{
   union
   {
      uint16 xy;         // 16-bit food coordinate ((y << 8) + x)
      POINT point;       // 2 8-bit food coordinates (x, y)
   } pos;				// maybe rename to position?
   uint8 size;           // pixel size of food
   uint8 points;         // value of food (not used)
   void (*draw)(struct food_struct* food, uint8 pen);
} FOOD;
typedef union							// snake segment object
{
	uint16 xy;
	POINT point;
} SNAKE;

#define COL(x)	(x*6+7+3)				// grid x value to LCD column
#define ROW(y)	(y*6+10+3)				// grid y value to LCD row

#endif /* SNAKE_H_ */
