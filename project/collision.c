#include <msp430.h>
#include "lcdutils.h"
#include "lcddraw.h"

//Square 1 
extern short drawPos[2], controlPos[2];
//square 2
extern short drawPosBall[2], controlPosBall[2];
//rectangle 1
extern short drawPos2[2], controlPos2[2];
//rectangle 2
extern short drawPos3[2], controlPos3[2];
//Ship
extern short drawPosShip[], controlPosShip[];

//bottom of ship is 21 and it's 11 tall
//squares are 10 by 10
//rectangles are 30 by 10

//Boolean used for collision check
extern char hitShape;

//following two methods compare the square position with ship position
void
square_1_check()
{
  for (short i = 0; i < 10;i++) {
    if ((drawPos[0]+i) == (drawPosShip[0]+i))
      hitShape = 1;
    if ((drawPos[1]+i) == (drawPosShip[1]+i))
      hitShape = 1;
  }
}

void
square_2_check()
{
  for (short i = 0; i < 10;i++) {
    if ((drawPosBall[0]+i) == (drawPosShip[0]+i))
      hitShape = 1;
    if ((drawPosBall[1]+i) == (drawPosShip[1]+i))
      hitShape = 1;
  }
}

//following two methods compare the rectangles position with ship position
void
rec_1_check()
{
  for (short i = 0; i < 32;i++) {
    if ((drawPos2[0]+i) == (drawPosShip[0]+i)) {
      hitShape = 1;
    }
  }
  for (short i = 0; i < 11;i++) {
    if ((drawPos[1]+i) == (drawPosShip[1]+i))
      hitShape = 1;
  }
}

void
rec_2_check()
{
  for (short i = 0; i < 32;i++) {
    if ((drawPos3[0]+i) == (drawPosShip[0]+i)) {
      hitShape = 1;
    }
  }
  for (short i = 0; i < 11;i++) {
    if ((drawPos3[1]+i) == (drawPosShip[1]+i)) {
      hitShape = 1;
    }
  }
}

//called to check for hit
void
ship_check_shapes()
{ 
  //Square check in rows that he squares can be
  if ((drawPosShip[1] > 30) && (drawPosShip[1] < 70)) {
    square_1_check();
    square_2_check();
  }
  //Rectangle check in rows that the rectangles can be
  if ((drawPosShip[1] > 90) && (drawPosShip[1] < 120)) {
    rec_1_check();
    rec_2_check();
  }
}
