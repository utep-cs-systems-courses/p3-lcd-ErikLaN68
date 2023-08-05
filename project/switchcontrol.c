#include <msp430.h>
#include "lcdutils.h"
#include "switchcontrol.h"
#include "shipdraw.h"
#include "buzzer.h"
#include "collision.h"

//All values needed for switches
extern char endGame;
extern int switches;
extern short drawPosShip[], controlPosShip[], colLimitsShip[], rowLimitsShip[], velocityShip;

//updates the ship
void
update_ship()
{
  screen_update_ship(controlPosShip, drawPosShip);
}

//resets the ship
void
rest_ship()
{
  controlPosShip[0] = (screenWidth>>1) - 10;
  controlPosShip[1] = (screenHeight - 5);
}

//controls all the switches that move the ship around
void
switch_controller()
{
  if (switches & SW1) {
    short oldCol = controlPosShip[0];
    short newCol = oldCol - velocityShip;
    short_play(400);
    __delay_cycles(500);
    if (newCol <= colLimitsShip[0])
      return;
    else
      controlPosShip[0] = newCol;
  }
  
  if (switches & SW2) {
    short oldRow = controlPosShip[1];
    short newRow = oldRow - velocityShip;
    buzzer_set_period(400);
    __delay_cycles(500);
    if (newRow <= rowLimitsShip[0]) {
      endGame = 1;
      return;
    }
    else {
      controlPosShip[1] = newRow;
    }
  }
  
  if (switches & SW3) {
    short oldRow = controlPosShip[1];
    short newRow = oldRow + velocityShip;
    buzzer_set_period(400);
    __delay_cycles(500);
    if (newRow >= rowLimitsShip[1])
      return;
    else
      controlPosShip[1] = newRow;
  }
  
  if (switches & SW4) {
    short oldCol2 = controlPosShip[0];
    short newCol2 = oldCol2 + velocityShip;
    buzzer_set_period(400);
    __delay_cycles(500);
    if (newCol2 >= colLimitsShip[1])
      return;
    else
      controlPosShip[0] = newCol2;
  } 
}
