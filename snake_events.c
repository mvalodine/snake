//******************************************************************************
//	snake_events.c
//
//  Author:			Paul Roper, Brigham Young University
//  Revisions:		1.0		11/25/2012	RBX430-1
// THIS IS MY OWN WORK - Michelle Stevens
//
//******************************************************************************
//
#include "msp430.h"
#include <stdlib.h>
#include "RBX430-1.h"
#include "RBX430_lcd.h"
#include "snake.h"
#include <time.h>

extern volatile uint16 sys_event;			// pending events
extern volatile uint16 move_cnt;			// snake speed
extern volatile uint16 WDT_Tone_Cnt;

extern volatile enum modes game_mode;		// 0=idle, 1=play, 2=next
extern volatile uint16 score;				// current score
extern volatile uint16 level;				// current level (1-4)
extern volatile uint16 direction;			// current move direction
extern volatile uint16 seconds;			    // current time left
											// maybe endgame unit and check unit jace

extern volatile uint8 head;					// head index into snake array
extern volatile uint8 tail;					// tail index into snake array
extern SNAKE snake[MAX_SNAKE];				// snake segments
extern FOOD* foodarray[MAX_FOOD];			//maybe change to 10 instead of MaxFood jace

extern const uint16 snake_text_image[];
extern const uint16 snake1_image[];

static uint8 move_right(SNAKE* head);		// move snake head right
static uint8 move_up(SNAKE* head);			// move snake head up
static uint8 move_left(SNAKE* head);		// move snake head left
static uint8 move_down(SNAKE* head);		// move snake head down
static void new_snake(uint16 length, uint8 dir);
static void delete_tail(void);
static void add_head(void);

//-- switch #1 event -----------------------------------------------------------
//
void doTone(uint16 tone, uint16 time)
{
   TBCCR0 = tone;                  // set beep frequency/duty cycle
   TBCCR2 = tone >> 1;             // 50% duty cycle
   WDT_Tone_Cnt = time;            // turn on speaker
   //while (WDT_Tone_Cnt);           // wait for tone off
   return;
} // end doTone
void doFancyTone(uint16 tone, uint16 time)
{
   TBCCR0 = tone;                  // set beep frequency/duty cycle
   TBCCR2 = tone >> 6;             // 50% duty cycle
   WDT_Tone_Cnt = time;            // turn on speaker
   while (WDT_Tone_Cnt);           // wait for tone off
   return;
} // end doTone
void doDitty(void)
{
	doFancyTone(TONE_C, 100);
	doFancyTone(TONE_E, 100);
	doFancyTone(TONE_F, 100);
	doFancyTone(TONE_C, 100);
}
void raspberry(void)
{
    doFancyTone(65535, 400);
    return;
}
void draw_square_food(FOOD* food, uint8 pen)
{
	lcd_square(COL(food->pos.point.x),
	ROW(food->pos.point.y),
	food->size, pen);
} // end draw_square_food
void draw_star_food(FOOD* food, uint8 pen)
{
	lcd_star(COL(food->pos.point.x),
	ROW(food->pos.point.y),
	food->size, pen);
} // end draw_star_food
void draw_circle_food(FOOD* food, uint8 pen)
{
	lcd_circle(COL(food->pos.point.x),
	ROW(food->pos.point.y),
	food->size, pen);
} // end draw_circle_food
void draw_triangle_food(FOOD* food, uint8 pen)
{
	lcd_triangle(COL(food->pos.point.x),
	ROW(food->pos.point.y),
	food->size, pen);
} // end draw_triangle_food

FOOD* createFood(FOOD* food, uint16 x, uint16 y, int size)
{
   // malloc a food struct
   if (!(food = (FOOD*)malloc(sizeof(FOOD)))) ERROR2(SYS_ERR_MALLOC);
   (food)->pos.point.x = x;              // add coordinates to food
   (food)->pos.point.y = y;             //   object
   /*int overstep = 0; // overstepping
	while(overstep!=1) // while overstepping
	{
		overstep = 1; // set it to not overstepping
		int k = 0;
		if (size>0)
		{
			for(; k<size; ++k) // if array of food and food have different position, add 1 to flag
			{
				if((foodarray[k]->pos.point.x== x) && (foodarray[k]->pos.point.y == y))
				{
					overstep = 0; // set it to overstepping, so it continues to
				}
			}
			foodarray[k]->pos.point.x = rand()%24; // do I need this in the for loop?
			foodarray[k]->pos.point.y = rand()%23;
		}
		else
		{
			foodarray[k]->pos.point.x = rand()%24;
			foodarray[k]->pos.point.y = rand()%23;
			overstep = 1; // set it to not overstepping cause obviously it's the only one in the array
		}
	}*/
   switch (rand() % 4)                     // randomly choose a type
   {
      case 0:                              // square food
         (food)->size = 2;                // 2 pixels in size
         (food)->draw = draw_square_food; // add function pointer to
         break;                            //   draw square
      case 1:                              // star food
		   (food)->size = 2;                // 2 pixels in size
		   (food)->draw = draw_star_food; // add function pointer to
		   break;                            //   draw square
      case 2:                              // circle food
		   (food)->size = 2;                // 2 pixels in size
		   (food)->draw = draw_circle_food; // add function pointer to
		   break;                            //   draw square
      case 3:                              // triangle food
		   (food)->size = 2;                // 2 pixels in size
		   (food)->draw = draw_triangle_food; // add function pointer to
		   break;                            //   draw square
   }
   ((food)->draw)(food, SINGLE);         // draw food
   return food;
} // end create food

void SWITCH_1_event(void)
{
	switch (game_mode)
	{
		case IDLE:
			sys_event |= NEW_GAME;
			break;

		case PLAY:
			if (direction != LEFT)
			{
				if (snake[head].point.x < X_MAX)
				{
					direction = RIGHT;
				}
			}
			break;

		case NEXT:
			sys_event |= START_LEVEL;
			break;
	}
	return;
} // end SWITCH_1_event


//-- switch #2 event -----------------------------------------------------------
//
void SWITCH_2_event(void)
{
	switch (game_mode)
	{
		/*case IDLE:
			sys_event |= NEW_GAME;
			break;*/
		case PLAY:
			if (direction != RIGHT)
			{
				if (snake[head].point.x > X_MIN)
				{
					direction = LEFT;
				}
			}
			break;
	}
} // end SWITCH_2_event


//-- switch #3 event -----------------------------------------------------------
//
void SWITCH_3_event(void)
{
	switch (game_mode)
	{
		/*case IDLE:
			sys_event |= NEW_GAME;
			break;*/
		case PLAY:
			if (direction != UP)
			{
				if (snake[head].point.x < Y_MAX)
				{
					direction = DOWN;
				}
			}
			break;
	}
} // end SWITCH_3_event


//-- switch #4 event -----------------------------------------------------------
//
void SWITCH_4_event(void)
{
	switch (game_mode)
	{
		/*case IDLE:
			sys_event |= NEW_GAME;
			break;*/
		case PLAY:
			if (direction != DOWN)
			{
				if (snake[head].point.x > Y_MIN)
				{
					direction = UP;
				}
			}
			break;
	}
} // end SWITCH_4_event



//-- next level event -----------------------------------------------------------
//
void NEXT_LEVEL_event(void) // clear food and rock arrays, free and set to null, for loop for food and rocks
{
	doDitty();
	int i;
	for(i = 0; i<MAX_FOOD; i++) // freefood
	{
		if(foodarray[i]!=NULL)
		{
			free(foodarray[i]);
			foodarray[i]= NULL;
		}
	}
	level++;
	if(level>2)
	{
		sys_event |= END_GAME;
	}
	// splash next level screen, lcd clear();
	/*if(level>4) // play next level ditty
	{
		sys_event = END_GAME;
	}*/
	//level++;
	sys_event |= START_LEVEL; // jace has end game
	//game_mode = NEXT;
} // end NEXT_LEVEL_event


//-- update LCD event -----------------------------------------------------------
//
void LCD_UPDATE_event(void)
{
	if(game_mode==PLAY)
	{
		lcd_cursor(7, 150);
		lcd_printf("SCORE: %d  LEVEL: %d  0:%d", score, level, seconds);
		if(seconds < 10 && seconds > 0)
		{
			lcd_cursor(7, 150);
			lcd_printf("SCORE: %d  LEVEL: %d  0:0%d", score, level, seconds);
		}
		else if(seconds == 0)
		{
			sys_event = END_GAME;
		}
	}
} // end LCD_UPDATE_event


//-- end game event -------------------------------------------------------------
//
void END_GAME_event(void)
{
	lcd_clear(); // clear lcd
	lcd_cursor(60, 80);
	lcd_printf("Final Score: %d", score);
	raspberry();
	seconds = 5; // this is really inneffecient
	while(seconds>0) // shouldn't do this
	{
	}
	int i = 0;
	for(; i<MAX_FOOD; i++) // check if null before free it
	{
		if(foodarray[i]!=NULL)
		{
			free(foodarray[i]);
			foodarray[i]= NULL;
		}
	}
	game_mode = IDLE;
	sys_event = NEW_GAME; // jace doesn't have this..
} // end END_GAME_event


//-- move snake event ----------------------------------------------------------
//
void MOVE_SNAKE_event(void) // check for collisons, make foods depending on level
{
	if (level > 0)
	{
		add_head();						// add head
		lcd_point(COL(snake[head].point.x), ROW(snake[head].point.y), PENTX);
		int flag = 0;
		int i = 0;
		for(; i<MAX_FOOD; i++)
		{
			if(snake[head].xy== foodarray[i]->pos.xy)
			{
				score+= level;
				//free(foodarray[i]); //j dog
				//foodarray[i]=NULL;
				sys_event |= LCD_UPDATE;
				doTone(TONE_C , 100);
				lcd_backlight(0);
				int j = 0;
				for(; j<30000; j++)
				{
				}
				lcd_backlight(1);
				flag = 1; // wont delete
				int overlap = 0; // j doesn't have
				if(level == 1)
				{
					while(overlap<2 )
					{
						//int j = tail-1;
						/*for (;j != head; (++j)%MAX_SNAKE)
						{
							if(foodarray[i]->pos.xy!=snake[j].xy) // if snake and food have different position, add 1 to flag
							{
								overlap++;
							}
						}*/
						int k = 0;
						for(; k<MAX_FOOD; k++) // if array of food and food have different position, add 1 to flag
						{
							if(foodarray[k]!= NULL && foodarray[k]->pos.xy!=foodarray[i]->pos.xy)
							{
								overlap++;
							}
						}
						foodarray[i]->pos.point.x = rand()%24;
						foodarray[i]->pos.point.y = rand()%23;
					}
					foodarray[i]->draw(foodarray[i], 1); // don't make it harder than it needs to be, really this should be in it's own check collision function
				}
				else if(level == 2)
				{
					if(foodarray[i]!=NULL)
					{
						free(foodarray[i]);
						foodarray[i]= NULL;
					}
				}
			}
		}
		if(flag==0) // maybe put if running into snake function or something...
		{
			delete_tail();
		}
		switch(level) // j doesn't have
		{
			case 1:
				if(score>9)
				{
					sys_event |= NEXT_LEVEL; // I had END_GAME before...
				}
				break;
			case 2:
				if(score>29)
				{
					sys_event |= NEXT_LEVEL;
				}
				break;
			/*case 3:
			case 4:// switch statements*/
		}
		int m = tail-1; // or just tail, depending on how they want it...
		for (;m != head; m = (m+1)%MAX_SNAKE)
		{

			if(snake[head].xy==snake[m].xy)
			{
				sys_event |= END_GAME;
			}

		}
	}
	return;
} // end MOVE_SNAKE_event


//-- start level event -----------------------------------------------------------
//
void START_LEVEL_event(void) // in new game make the pointers = NULL, in next, if it doesnt equal null, then free it. then set it equal to null.
{
	lcd_clear();
	lcd_backlight(1);					// turn on backlight
	lcd_rectangle(0,0,160,10,17);
	lcd_rectangle(0,10,7,140, 17);
	lcd_rectangle(152,10,8,140, 17);
	lcd_rectangle(0,149,160,11, 17);
	lcd_mode(2);
	lcd_cursor(7, 1);
	lcd_printf("UP   DOWN   LEFT   RIGHT");
	//new_snake(START_SCORE, RIGHT);
	int k = 0;
	for(; k<MAX_FOOD; k++) // freefood
	{
		if(foodarray[k]!=NULL)
		{
			free(foodarray[k]);
			foodarray[k]= NULL;
		}
	}
	if(level == 1)
	{
		int i = 0;
		for(; i<MAX_FOOD; i++) // first free foods and then create foods...
		{
		FOOD* foods = NULL;
		foodarray[i] = createFood(foods, (rand()%24), (rand()%23), i);
		}
		seconds = TIME_1_LIMIT;
		move_cnt = WDT_MOVE1;
	}
	else if(level == 2)
	{
		int j = 0;
		for(; j<MAX_FOOD; j++) // first free foods and then create foods...
		{
		FOOD* foods = NULL;
		foodarray[j] = createFood(foods, (rand()%24), (rand()%23), j);
		}
		seconds = TIME_2_LIMIT;
		move_cnt = WDT_MOVE2;
	}
	else if(level == 3)
	{
		seconds = TIME_3_LIMIT;
		move_cnt = WDT_MOVE3;
	}
	else if(level == 4)
	{
		seconds = TIME_2_LIMIT;
		move_cnt = WDT_MOVE4;
	}
	game_mode = PLAY;
	return;
} // end START_LEVEL_event


//-- new game event ------------------------------------------------------------
//
void NEW_GAME_event(void)
{
	lcd_clear();						// clear lcd
	lcd_backlight(1);					// turn on backlight
	lcd_wordImage(snake1_image, (159-60)/2, 60, 1);
	lcd_wordImage(snake_text_image, (159-111)/2, 20, 1);
	level = 1; // set initial level
	score = 0; // set initial score to zero
	seconds = TIME_1_LIMIT; // set initial time limit for level 1
	new_snake(START_SCORE, RIGHT); // make a new snake
	game_mode = NEXT;
	//sys_event = START_LEVEL; // start the level (if mode is not idle, aka switch is pressed)
	return;
} // end NEW_GAME_event


//-- new snake -----------------------------------------------------------------
//
void new_snake(uint16 length, uint8 dir)
{
	int i;
	head = 0;
	tail = 0;
	snake[head].point.x = 0;
	snake[head].point.y = 0;
	direction = dir;

	// build snake
	score = length;
	for (i = score - 1; i > 0; --i)
	{
		add_head();
	}
	return;
} // end new_snake


//-- delete_tail  --------------------------------------------------------------
//
void delete_tail(void)
{
	lcd_point(COL(snake[tail].point.x), ROW(snake[tail].point.y), PENTX_OFF);
	tail = (tail + 1) & (~MAX_SNAKE);
} // end delete_tail


//-- add_head  -----------------------------------------------------------------
//
void add_head(void)
{
	static uint8 (*mFuncs[])(SNAKE*) =	// move head function pointers
	             { move_right, move_up, move_left, move_down };
	uint8 new_head = (head + 1) & (~MAX_SNAKE);
	snake[new_head] = snake[head];		// set new head to previous head
	head = new_head;
	// iterate until valid move
	while ((*mFuncs[direction])(&snake[head]));
} // end add_head


//-- move snake head right -----------------------------------------------------
//
uint8 move_right(SNAKE* head)
{
	if ((head->point.x + 1) < X_MAX)		// room to move right?
	{
		++(head->point.x);					// y, move right
		return FALSE;
	}
	if (level != 2)							// n, right fence
	{
		if (level > 2) sys_event = END_GAME;
		head->point.x = 0;					// wrap around
		return FALSE;
	}
	if (head->point.y) direction = DOWN;	// move up/down
	else direction = UP;
	return TRUE;
} // end move_right


//-- move snake head up --------------------------------------------------------
//
uint8 move_up(SNAKE* head)
{
	if ((head->point.y + 1) < Y_MAX)
	{
		++(head->point.y);					// move up
		return FALSE;
	}
	if (level != 2)							// top fence
	{
		if (level > 2) sys_event = END_GAME;
		head->point.y = 0;					// wrap around
		return FALSE;
	}
	if (head->point.x) direction = LEFT;	// move left/right
	else direction = RIGHT;
	return TRUE;
} // end move_up


//-- move snake head left ------------------------------------------------------
//
uint8 move_left(SNAKE* head)
{
	if (head->point.x)
	{
		--(head->point.x);					// move left
		return FALSE;
	}
	if (level != 2)							// left fence
	{
		if (level > 2) sys_event = END_GAME;
		head->point.x = X_MAX - 1;			// wrap around
		return FALSE;
	}
	if (head->point.y) direction = DOWN;	// move down/up
	else direction = UP;
	return TRUE;
} // end move_left


//-- move snake head down ------------------------------------------------------
//
uint8 move_down(SNAKE* head)
{
	if (head->point.y)
	{
		--(head->point.y);					// move down
		return FALSE;
	}
	if (level != 2)							// bottom fence
	{
		if (level > 2) sys_event = END_GAME;
		head->point.y = Y_MAX - 1;			// wrap around
		return FALSE;
	}
	if (head->point.x) direction = LEFT;	// move left/right
	else direction = RIGHT;
	return TRUE;
} // end move_down
