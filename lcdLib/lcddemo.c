/** \file lcddemo.c
 *  \brief A simple demo that draws a string and square
 */

#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

int max(int a, int b) {return a > b ? a : b; }
int min(int a, int b) {return a > b ? b : a; }

void drawHourGlass(int controlCol, int controlRow, int size, short color)
{
  int c = 0;
  int r = 0;
  for (int j = 0; j < size; j++) {
    int upper = min(-r, +r);
    int lower = max(-r, +r);
    int drawCol = c;
    for (int i = upper; i <= lower; i++) {
      drawPixel(controlCol+drawCol,controlRow+i,color);
      drawPixel(controlCol-drawCol,controlRow+i,color);
    }
    c += 1;
    r += 1;
  }
}

/** Initializes everything, clears the screen, draws "hello" and a square */
int
main()
{
  configureClocks();
  lcd_init();
  u_char width = screenWidth, height = screenHeight;

  clearScreen(COLOR_BLACK);

  drawString8x12(40,20, "Erik LaNeave", COLOR_GREEN, COLOR_BLACK);
  drawChar8x12(20,20,'E', COLOR_GREEN, COLOR_BLACK);
  //fillRectangle(30,30, 60, 60, COLOR_ORANGE);
  
  drawHourGlass(screenWidth >> 1, screenHeight >> 1, 15, COLOR_WHITE);
  
}

/*
void drawHourGlass(int controlCol, int ControlRow, int size, u_int16 color)
{
  int c = 0;
  int r = 0;
  for (int j = 0; j < size; j++) {
    int upper = min(-r, +r);
    int lower = max(-r, +r);
    int drawCol = c;
    for (int i = upper; i <= lower; i++) {
      drawPixel(controlCol+drawCol,controlRow+i,color);
      drawPixel(controlCol-drawCol,controlRow+i,color);
    }
    c += 1;
    r += 1;
  }
}*/
