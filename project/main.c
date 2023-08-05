#include <msp430.h>
#include <libTimer.h>
#include <stdlib.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "buzzer.h"
#include "background.h"
#include "switchcontrol.h"
#include "shapecontrol.h"
#include "collision.h"
#include "switchstate.h"

// WARNING: LCD DISPLAY USES P1.0.  Do not touch!!! 

#define LED BIT6		/* note that bit zero req'd for display */

/*#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8*/

#define SWITCHES 15

//axis zero for col, axis 1 for row
//square 1 
short drawPos[2] = {10,10}, controlPos[2] = {1, 40};
short colVelocity = 1, colLimits[2] = {0, screenWidth-10};
//rectangle 1
short drawPos2[2] = {10,10}, controlPos2[2] = {1, 110};
short colVelocity2 = 2, colLimits2[2] = {0, (screenWidth>>1)-28};
//square 2
short drawPosBall[2] = {10,10}, controlPosBall[2] = {screenWidth-10, 50};
short colVelocityBall = 1, colLimitsBall[2] = {0, screenWidth-10};
//rectangle 2
short drawPos3[2] = {10,10}, controlPos3[2] = {screenWidth - 30, 110};
short colVelocity3 = 2, colLimits3[2] = {(screenWidth>>1)-1, screenWidth-29};
//ship
short drawPosShip[2] = {1,1}, controlPosShip[2] = {((screenWidth >> 1) - 10), (screenHeight-5)};
short velocityShip = 1, colLimitsShip[2] = {1,screenWidth-21}, rowLimitsShip[2] = {11,screenHeight-5};

//boolean variables for state control
short redrawScreen = 1;
char endGame = 0;
char hitShape = 0;

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

//startDisplay used to control how long the start screen is shown for and how many secs have passed till game end
int startDisplay = 0;
//boolean and state control variable
char showStart = 1;
char state = 1;

void
wdt_c_handler()
{
  //call to switch statement
  switch_controller();
 
  static int secCount = 0;
  //Count times for different operation
  secCount ++;
  startDisplay++;
  //about 4 secs
  if (startDisplay > 900) {
    showStart = 0;
  }
  //updates shapes once secCount > 5
  if (secCount >= 5) {  
    shape_controller();
    secCount = 0;
  }

  //turns buzzer off and redraws the screen
  buzzer_set_period(0);
  redrawScreen = 1;
}

//Used to find how much time is passed
int secEnd = 0;
char *secP;

//takes startDisplay and divids by 250 for num of sec
//resets the startDisplay variable
void
endTime()
{
  secEnd = startDisplay/250;
  startDisplay = 0;
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
  //for state control
  state_switch(state);
  
  while (1) {
    if (redrawScreen) {
      redrawScreen = 0;
      update_ship();
      update_shapes();
      ship_check_shapes();
    }
    //Hit check and end game check
    if (endGame || hitShape) {
      //check state
      if(endGame)
	state = 2;
      else
	state = 3;
      //finds time
      endTime();
      //has to be here
      char sec[50] = "";
      //start screen and endscreen are up for around 4sec each
      itoa(secEnd-4,sec,10);
      secP = sec;
      //call to state_switch
      state_switch(state);
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
