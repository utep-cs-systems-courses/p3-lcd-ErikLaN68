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


// axis zero for col, axis 1 for row

short drawPos[2] = {10,10}, controlPos[2] = {2, 10};
short colVelocity = 2, colLimits[2] = {1, screenWidth};

void
draw_ball(int col, int row, unsigned short color)
{
  fillRectangle(col-1, row-1, 5, 5, color);
}


void
screen_update_ball()
{
  for (char axis = 0; axis < 2; axis ++) 
    if (drawPos[axis] != controlPos[axis]) /* position changed? */
      goto redraw;
  return;			/* nothing to do */
 redraw:
  draw_ball(drawPos[0], drawPos[1], COLOR_BLACK); /* erase */
  for (char axis = 0; axis < 2; axis ++) 
    drawPos[axis] = controlPos[axis];
  draw_ball(drawPos[0], drawPos[1], COLOR_RED); /* draw */
}

short drawPosShip[2] = {1,1}, controlPosShip[2] = {((screenWidth >> 1) - 10), (screenHeight-5)};
short colVelocityShip = 1, colLimitsShip[2] = {1,screenWidth-21}, rowLimitShip[2] = {11,screenHeight-5};
//draw_ship((screenWidth >> 1) - 10, screenHeight-5, COLOR_RED);

void
screen_update_ship()
{
  for (char axis = 0; axis < 2; axis ++) 
    if (drawPosShip[axis] != controlPosShip[axis]) /* position changed? */
      goto redraw;
  return;			/* nothing to do */
 redraw:
  draw_ship(drawPosShip[0], drawPosShip[1], COLOR_BLACK); /* erase */
  for (char axis = 0; axis < 2; axis ++)
    drawPosShip[axis] = controlPosShip[axis];
  draw_ship(drawPosShip[0], drawPosShip[1], COLOR_RED); /* draw */
}  

short redrawScreen = 1;
u_int controlFontColor = COLOR_GREEN;

void wdt_c_handler()
{
  static int secCount = 0;
  secCount ++;
  if (secCount >= 10) {//10/sec
    
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
    
    /*{				//move ball
      short oldCol = controlPosShip[0];
      short newCol = oldCol + colVelocityShip;
      if (newCol <= colLimitsShip[0] || newCol >= colLimitsShip[1])
	colVelocityShip = -colVelocityShip;
      else
	controlPosShip[0] = newCol;
    }

    {				//update hourglass
      if (switches & SW3) green = (green + 1) % 64;
      if (switches & SW2) blue = (blue + 2) % 32;
      if (switches & SW1) red = (red - 3) % 32;
      if (step <= 30)
	step ++;
      else
	step = 0;
      secCount = 0;
    }
    if (switches & SW4) return;*/
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
  screen_update_ball();
  screen_update_ship();
}
   


void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {	      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;		      /* clear pending sw interrupts */
    switch_interrupt_handler();	/* single handler for all switches */
  }
}
