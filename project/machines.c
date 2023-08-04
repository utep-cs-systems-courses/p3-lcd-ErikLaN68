#include <msp430.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "background.h"
#include "switchcontrol.h"

extern char showStart;
extern char hitShape;
extern char endGame;

void
state_1()
{
  //color of start screen
  clearScreen(COLOR_DARK_VIOLE);
  while (showStart) {
    start_page();
  }
}

void
state_2()
{
  char startStop = 15;
  while (startStop > 0) {
    end_page();
    startStop--;
  }
  clearScreen(COLOR_BLACK);
  rest_ship();
  hitShape = 0;
  endGame = 0;
}

void
state_3()
{
  char startStop = 15;
  while (startStop > 0) {
    lost_page();
    startStop--;
  }
  clearScreen(COLOR_BLACK);
  rest_ship();
  hitShape = 0;
  endGame = 0;
}
