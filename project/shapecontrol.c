#include <msp430.h>
#include "lcdutils.h"
#include "background.h"
#include "collision.h"

//square 1 
extern short drawPos[2], controlPos[2], colVelocity, colLimits[2];
//square 2
extern short drawPosBall[2], controlPosBall[2], colVelocityBall, colLimitsBall[2];
//rectangle 1
extern short drawPos2[2], controlPos2[2], colVelocity2, colLimits2[2];
//rectangle 2
extern short drawPos3[2], controlPos3[2], colVelocity3, colLimits3[2];

//checks for updates in the shape positions
void
update_shapes()
{
  screen_update_shape(controlPos, drawPos, COLOR_BLUE, 1);
  screen_update_shape(controlPosBall, drawPosBall,COLOR_WHITE, 1);
  screen_update_shape(controlPos2, drawPos2, COLOR_PURPLE, 2);
  screen_update_shape(controlPos3, drawPos3, COLOR_PURPLE, 2);
  
}

//checks all shapes and if the limits have been hit
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
