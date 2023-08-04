#include <msp430.h>
#include "lcdutils.h"
#include "lcddraw.h"

//Square 1 
extern short drawPos[2], controlPos[2], colVelocity, colLimits[2];
//square 2
extern short drawPosBall[2], controlPosBall[2], colVelocityBall, colLimitsBall[2];
//rectangle 1
extern short drawPos2[2], controlPos2[2], colVelocity2, colLimits2[2];
//rectangle 2
extern short drawPos3[2], controlPos3[2], colVelocity3, colLimits3[2];
//Ship
extern short drawPosShip[], controlPosShip[], colLimitsShip[], rowLimitsShip[], velocityShip;

//bottom of ship is 21 and it's 11 tall
//squares are 10 by 10
//rectangles are 30 by 10

extern char hitShape;

//short shipAreaC = controlPosShip[0]+21;
//short shipAreaR = controlPosShip[1]+11;

void
square_1_check()
{
  for (short i = 0; i < 10;i++) {
    if ((controlPos[0]+i) == (controlPosShip[0]+i)) {
      hitShape = 1;
    }
  }
  for (short i = 0; i < 10;i++) {
    if ((controlPos[1]+i) == (controlPosShip[1]+i)) {
      hitShape = 1;
    }
  }
}

void
square_2_check()
{
  for (short i = 0; i < 10;i++) {
    if ((controlPosBall[0]+i) == (controlPosShip[0]+i)) {
      hitShape = 1;
    }
  }
  for (short i = 0; i < 10;i++) {
    if ((controlPosBall[1]+i) == (controlPosShip[1]+i)) {
      hitShape = 1;
    }
  }
}

void
rec_1_check()
{
  for (short i = 0; i < 25;i++) {
    if ((controlPos2[0]+i) == (controlPosShip[0]+i)) {
      hitShape = 1;
    }
  }
  for (short i = 0; i < 11;i++) {
    if ((controlPos2[1]+i) == (controlPosShip[1]+i))
      hitShape = 1;
  }
}

void
rec_2_check()
{
  for (short i = 0; i < 25;i++) {
    if ((controlPos3[0]+i) == (controlPosShip[0]+i)) {
      hitShape = 1;
    }
  }
  for (short i = 0; i < 11;i++) {
    if ((controlPos3[1]+i) == (controlPosShip[1]+i)) {
      hitShape = 1;
    }
  }
}

void
ship_check_shapes()
{ 
  //Square check
  if ((controlPosShip[1] > 30) && (controlPosShip[1] < 70)) {
    square_1_check();
    square_2_check();
  }
  //Rectangle check
  if ((controlPosShip[1] > 90) && (controlPosShip[1] < 140)) {
    rec_1_check();
    rec_2_check();
  }
}
