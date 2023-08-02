#include <msp430.h>
#include <libTimer.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "shipdraw.h"
#include "buzzer.h"
#include "background.h"

// WARNING: LCD DISPLAY USES P1.0.  Do not touch!!! 

#define LED BIT6		/* note that bit zero req'd for display */

#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8

#define SWITCHES 15

/*char blue = 31, green = 0, red = 31;
  unsigned char step = 0;*/

static char 
switch_update_interrupt_sense()
{
  char p2val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);	/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);	/* if switch down, sense up */
  return p2val;
}

void 
switch_init()			/* setup switch */
{  
  P2REN |= SWITCHES;		/* enables resistors for switches */
  P2IE |= SWITCHES;		/* enable interrupts from switches */
  P2OUT |= SWITCHES;		/* pull-ups for switches */
  P2DIR &= ~SWITCHES;		/* set switches' bits for input */
  switch_update_interrupt_sense();
}

int switches = 0;

void
switch_interrupt_handler()
{
  char p2val = switch_update_interrupt_sense();
  switches = ~p2val & SWITCHES;
}

//axis zero for col, axis 1 for row
//square 1 
short drawPos[2] = {10,10}, controlPos[2] = {1, 40};
short colVelocity = 1, colLimits[2] = {0, screenWidth-10};

//rectangle 1
short drawPos2[2] = {10,10}, controlPos2[2] = {1, 110};
short colVelocity2 = 2, colLimits2[2] = {0, (screenWidth>>1)-28};

//rectangle 2
short drawPos3[2] = {10,10}, controlPos3[2] = {screenWidth - 30, 110};
short colVelocity3 = 2, colLimits3[2] = {(screenWidth>>1)-1, screenWidth-29};

//ship
short drawPosShip[2] = {1,1}, controlPosShip[2] = {((screenWidth >> 1) - 10), (screenHeight-5)};
short colVelocityShip = 1, colLimitsShip[2] = {1,screenWidth-21}, rowLimitShip[2] = {11,screenHeight-5};

//square 2
short drawPosBall[2] = {10,10}, controlPosBall[2] = {screenWidth-10, 50};
short colVelocityBall = 1, colLimitsBall[2] = {0, screenWidth-10};

short redrawScreen = 1;
//u_int controlFontColor = COLOR_GREEN;
char endGame = 0;

void
switch_controller()
{
  //short_play(1200);
  if (switches & SW1) {
    short oldCol = controlPosShip[0];
    short newCol = oldCol - colVelocityShip;
    //buzzer_set_period(1000);
    short_play(400);
    if (newCol <= colLimitsShip[0])
      return;
    else
      controlPosShip[0] = newCol;
  }
  
  if (switches & SW2) {
    short oldRow = controlPosShip[1];
    short newRow = oldRow - colVelocityShip;
    buzzer_set_period(400);
    __delay_cycles(500);
    if (newRow <= rowLimitShip[0]) {
      endGame = 1;
      return;
    }
    else {
      controlPosShip[1] = newRow;
    }
  }
  
  if (switches & SW3) {
    short oldRow = controlPosShip[1];
    short newRow = oldRow + colVelocityShip;
    short_play(1500);
    if (newRow >= rowLimitShip[1])
      return;
    else
      controlPosShip[1] = newRow;
  }
  
  if (switches & SW4) {
    short oldCol2 = controlPosShip[0];
    short newCol2 = oldCol2 + colVelocityShip;
    //short_play(1200);
    if (newCol2 >= colLimitsShip[1])
      return;
    else
      controlPosShip[0] = newCol2;
    buzzer_set_period(400);
    __delay_cycles(500);
  } 
}

void
shape_controller()
{
  short oldColBall = controlPos[0];
  short newColBall = oldColBall + colVelocity;
  if (newColBall <= colLimits[0] || newColBall >= colLimits[1])
    colVelocity = -colVelocity;
  else
    controlPos[0] = newColBall;
  
  short oldColBall2 = controlPosBall[0];
  short newColBall2 = oldColBall2 + colVelocityBall;
  if (newColBall2 <= colLimitsBall[0] || newColBall2 >= colLimitsBall[1])
    colVelocityBall = -colVelocityBall;
  else
    controlPosBall[0] = newColBall2;
  
  short oldCol2 = controlPos2[0];
  short newCol2 = oldCol2 + colVelocity2;
  if (newCol2 <= colLimits2[0] || newCol2 >= colLimits2[1])
    colVelocity2 = -colVelocity2;
  else
    controlPos2[0] = newCol2;

  short oldCol3 = controlPos3[0];
  short newCol3 = oldCol3 + colVelocity3;
  if (newCol3 <= colLimits3[0] || newCol3 >= colLimits3[1])
    colVelocity3 = -colVelocity3;
  else
    controlPos3[0] = newCol3;
}

int startDisplay = 0;
char showStart = 1;

void
wdt_c_handler()
{
  switch_controller();
  
  static int secCount = 0;
  secCount ++;
  startDisplay++;
  if (startDisplay > 960) {
    showStart = 0;
  }
  if (secCount >= 5) {  
    shape_controller();
    secCount = 0;
  }
  
  buzzer_set_period(0);
  redrawScreen = 1;
}

//What does this do
void update_shapes();
void
update_shapes()
{
  screen_update_ship(controlPosShip, drawPosShip);
  screen_update_shape(controlPos, drawPos, COLOR_BLUE, 1);
  screen_update_shape(controlPosBall, drawPosBall,COLOR_WHITE, 1);
  screen_update_shape(controlPos2, drawPos2, COLOR_PURPLE, 2);
  screen_update_shape(controlPos3, drawPos3, COLOR_PURPLE, 2);
}

int secEnd = 0;
int startStop = 0;

void
endTime()
{
  secEnd = startDisplay/240;
}

void
main()
{
  P1DIR |= LED;		/**< Green led on when CPU on */
  P1OUT |= LED;
  configureClocks();
  lcd_init();
  switch_init();
  buzzer_init();
  
  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */

  clearScreen(COLOR_DARK_VIOLE);
  while (showStart) {
    drawString5x7(10,screenHeight>>1,"Triangle Dash",COLOR_WHITE,COLOR_DARK_VIOLE);
  }
  
  clearScreen(COLOR_BLACK);
  while (1) {   /* forever */
    //shows the center of the screen
    /*for (int i = 0; i < 40; i++)  
      drawPixel(screenWidth >> 1, (screenHeight-5)-i, COLOR_WHITE);*/
    if (redrawScreen) {
      redrawScreen = 0;
      update_shapes();
    }
    if (endGame) {
      startStop = startDisplay;
      clearScreen(COLOR_DARK_VIOLE);
      endTime();
      int startStop = 200;
      char sec[50] = "";
      itoa(secEnd-5,sec,10);
      char *secP = sec;
      startDisplay = 0;
      while (startStop > 0) {
	lazer(10);
	drawString5x7(20,(screenHeight>>1)-50,"YOU WON!!",COLOR_WHITE,COLOR_DARK_VIOLE);
	drawString5x7(screenWidth>>1,screenHeight>>1,secP,COLOR_WHITE,COLOR_DARK_VIOLE);
	startStop--;
      }
      clearScreen(COLOR_BLACK);
      endGame = 0;
      controlPosShip[1] = ((screenWidth >> 1) - 10);
      controlPosShip[2] = (screenHeight-5);
      update_shapes();
      //screen_update_ship(controlPosShip, drawPosShip);
    }
    P1OUT &= ~LED;	/* led off */
    or_sr(0x10);	/**< CPU OFF */
    P1OUT |= LED;	/* led on */
  }
}
 
void
__interrupt_vec(PORT2_VECTOR) Port_2()
{
  if (P2IFG & SWITCHES) {	      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;		      /* clear pending sw interrupts */
    switch_interrupt_handler();	/* single handler for all switches */
  }
}
