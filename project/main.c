#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "shipdraw.h"

// WARNING: LCD DISPLAY USES P1.0.  Do not touch!!! 

#define LED BIT6		/* note that bit zero req'd for display */

#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8

#define SWITCHES 15

char blue = 31, green = 0, red = 31;
unsigned char step = 0;

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

short drawPos2[2] = {10,10}, controlPos2[2] = {1, 110};
short colVelocity2 = 2, colLimits2[2] = {0, (screenWidth>>1)-30};

short drawPosShip[2] = {1,1}, controlPosShip[2] = {((screenWidth >> 1) - 10), (screenHeight-5)};
short colVelocityShip = 1, colLimitsShip[2] = {1,screenWidth-21}, rowLimitShip[2] = {11,screenHeight-5};

short drawPosBall[2] = {10,10}, controlPosBall[2] = {screenWidth-10, 50};
short colVelocityBall = 1, colLimitsBall[2] = {0, screenWidth-10};

//draw_ship((screenWidth >> 1) - 10, screenHeight-5, COLOR_RED);

short redrawScreen = 1;
u_int controlFontColor = COLOR_GREEN;

void wdt_c_handler()
{
  if (switches & SW1) {
      short oldCol = controlPosShip[0];
      short newCol = oldCol - colVelocityShip;
      if (newCol <= colLimitsShip[0])
	return;
      else
	controlPosShip[0] = newCol;
    }
    
    if (switches & SW2) {
      short oldRow = controlPosShip[1];
      short newRow = oldRow - colVelocityShip;
      if (newRow <= rowLimitShip[0])
	return;
      else
	controlPosShip[1] = newRow;
    }
    
    if (switches & SW3) {
      short oldRow = controlPosShip[1];
      short newRow = oldRow + colVelocityShip;
      if (newRow >= rowLimitShip[1])
	return;
      else
	controlPosShip[1] = newRow;
    }
    
    if (switches & SW4) {
      short oldCol2 = controlPosShip[0];
      short newCol2 = oldCol2 + colVelocityShip;
      if (newCol2 >= colLimitsShip[1])
	return;
      else
	controlPosShip[0] = newCol2;
    }
  static int secCount = 0;
  secCount ++;
  if (secCount >= 5) {//10/sec
    {				//move ball
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
      secCount = 0;
    }
    redrawScreen = 1;
  }
}

//What does this do
void update_shape();

void main()
{
  
  P1DIR |= LED;		/**< Green led on when CPU on */
  P1OUT |= LED;
  configureClocks();
  lcd_init();
  switch_init();
  
  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */
  
  clearScreen(COLOR_BLACK);
  //draw_ship(screenWidth >> 1, screenHeight >> 1, COLOR_RED);
  while (1) {   /* forever */
    //draw_ship((screenWidth >> 1) - 10, screenHeight-5, COLOR_RED);
    
    //shows the center of the screen
    /*for (int i = 0; i < 20; i++)  
      drawPixel(screenWidth >> 1, (screenHeight-5)-i, COLOR_WHITE);*/
      
    if (redrawScreen) {
      redrawScreen = 0;
      update_shape();
      }
    P1OUT &= ~LED;	/* led off */
    or_sr(0x10);	/**< CPU OFF */
    P1OUT |= LED;	/* led on */
  }
}


void
screen_update_hourglass()
{
  static unsigned char row = screenHeight / 2, col = screenWidth / 2;
  static char lastStep = 0;
  
  if (step == 0 || (lastStep > step)) {
    clearScreen(COLOR_BLUE);
    lastStep = 0;
  } else {
    for (; lastStep <= step; lastStep++) {
      int startCol = col - lastStep;
      int endCol = col + lastStep;
      int width = 1 + endCol - startCol;
      
      // a color in this BGR encoding is BBBB BGGG GGGR RRRR
      unsigned int color = (blue << 11) | (green << 5) | red;
      
      fillRectangle(startCol, row+lastStep, width, 1, color);
      fillRectangle(startCol, row-lastStep, width, 1, color);
    }
  }
}

    
void
update_shape()
{
  screen_update_ship(controlPosShip, drawPosShip);
  screen_update_shape(controlPos, drawPos, COLOR_BLUE, 1);
  screen_update_shape(controlPosBall, drawPosBall,COLOR_WHITE, 1);
  screen_update_shape(controlPos2, drawPos2, COLOR_PURPLE, 2);
}
   


void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {	      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;		      /* clear pending sw interrupts */
    switch_interrupt_handler();	/* single handler for all switches */
  }
}
