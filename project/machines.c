#include <msp430.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "background.h"
#include "switchcontrol.h"

extern char showStart;
extern char hitShape;
extern char endGame;

//shows start page
void
state_1()
{
  //color of start screen
  clearScreen(COLOR_DARK_VIOLE);
  while (showStart) {
    start_page();
  }
  //clears the screen
  clearScreen(COLOR_BLACK);
}

//shows the end game page
void
state_2()
{
  clearScreen(COLOR_DARK_VIOLE);
  char startStop = 15;
  while (startStop > 0) {
    end_page();
    startStop--;
  }
  //resting everything
  clearScreen(COLOR_BLACK);
  rest_ship();
  hitShape = 0;
  endGame = 0;
}

//shows the collision page
void
state_3()
{
  clearScreen(COLOR_DARK_VIOLE);
  char startStop = 15;
  while (startStop > 0) {
    lost_page();
    startStop--;
  }
  //resting everything
  clearScreen(COLOR_BLACK);
  rest_ship();
  hitShape = 0;
  endGame = 0;
}
