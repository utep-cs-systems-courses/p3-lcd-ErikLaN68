#include "lcdutils.h"
#include "lcddraw.h"

void
draw_shape_1(int col, int row, unsigned short color)
{
  fillRectangle(col, row, 10, 10, color);
}

void
draw_shape_2(int col, int row, unsigned short color)
{
  fillRectangle(col, row, 30, 10, color);
}

void
draw_shape_3(int col, int row, unsigned short color)
{
  
}

void
shape_pick(short controlPos[], short drawPos[], unsigned short color, char control)
{
  switch (control) {
  case 1:
    draw_shape_1(drawPos[0], drawPos[1], color);
    break;
  case 2:
    draw_shape_2(drawPos[0], drawPos[1], color);
    break;
  case 3:
    draw_shape_3(drawPos[0], drawPos[1], color);
    break;
  default:
    break;
  }
}

void
screen_update_shape(short controlPos[], short drawPos[], unsigned short color, char control)
{
  for (char axis = 0; axis < 2; axis ++) 
    if (drawPos[axis] != controlPos[axis]) /* position changed? */
      goto redraw;
  return;			/* nothing to do */
 redraw:
  shape_pick(controlPos, drawPos, COLOR_BLACK, control); /* erase */
  for (char axis = 0; axis < 2; axis ++) 
    drawPos[axis] = controlPos[axis];
  shape_pick(controlPos, drawPos, color, control); /* draw */
}
