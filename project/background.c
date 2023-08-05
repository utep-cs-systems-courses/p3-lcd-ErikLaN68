#include "lcdutils.h"
#include "lcddraw.h"
#include "buzzer.h"

//draws the squares
void
draw_shape_1(int col, int row, unsigned short color)
{
  fillRectangle(col, row, 10, 10, color);
}

//draws the rectangles
void
draw_shape_2(int col, int row, unsigned short color)
{
  fillRectangle(col, row, 30, 10, color);
}

//text spots for start page
void
start_page()
{
  drawString8x12(5,20,"Triangle",COLOR_WHITE,COLOR_DARK_VIOLE);
  drawString8x12(40,35,"Dash",COLOR_WHITE,COLOR_DARK_VIOLE);
  drawString5x7(2,65,"Get to the other side",COLOR_WHITE,COLOR_DARK_VIOLE);
  drawString5x7(3,90,"Watch out for shapes!",COLOR_WHITE,COLOR_DARK_VIOLE);
  drawString8x12(30,120,"Have Fun",COLOR_WHITE,COLOR_DARK_VIOLE);
}

//pointer that has the number of seconds
extern char *secP;
//text for end page

void
end_page()
{
  drawString8x12(10,20,"YOU WON!!",COLOR_WHITE,COLOR_DARK_VIOLE);
  drawString8x12(10,40,"It took ",COLOR_WHITE,COLOR_DARK_VIOLE);
  drawString8x12(65,40,secP,COLOR_WHITE,COLOR_DARK_VIOLE);
  drawString8x12(85,40,"sec",COLOR_WHITE,COLOR_DARK_VIOLE);
  drawString8x12(10,55,"to get across",COLOR_WHITE,COLOR_DARK_VIOLE);
  drawString5x7(3,100,"Try going back across",COLOR_WHITE,COLOR_DARK_VIOLE);
  drawString5x7(3,115,"to win again!",COLOR_WHITE,COLOR_DARK_VIOLE);
}

//lost page uses the same sec pointer
void
lost_page()
{
  drawString8x12(10,20,"You Lost!",COLOR_WHITE,COLOR_DARK_VIOLE);
  drawString8x12(10,40,"It took ",COLOR_WHITE,COLOR_DARK_VIOLE);
  drawString8x12(65,40,secP,COLOR_WHITE,COLOR_DARK_VIOLE);
  drawString8x12(85,40,"sec",COLOR_WHITE,COLOR_DARK_VIOLE);
  drawString8x12(10,55,"to lose",COLOR_WHITE,COLOR_DARK_VIOLE);
  drawString5x7(3,100,"Try going back across",COLOR_WHITE,COLOR_DARK_VIOLE);
  drawString5x7(3,115,"to win!",COLOR_WHITE,COLOR_DARK_VIOLE);
}

//used to know which shape to draw in the screen update method
void
shape_pick(short drawPos[], unsigned short color, char control)
{
  switch (control) {
  case 1:
    draw_shape_1(drawPos[0], drawPos[1], color);
    break;
  case 2:
    draw_shape_2(drawPos[0], drawPos[1], color);
    break;
  default:
    break;
  }
}

//updates the rectangles and squares
void
screen_update_shape(short controlPos[], short drawPos[], unsigned short color, char control)
{
  for (char axis = 0; axis < 2; axis ++) 
    if (drawPos[axis] != controlPos[axis]) /* position changed? */
      goto redraw;
  return;			/* nothing to do */
 redraw:
  shape_pick(drawPos, COLOR_BLACK, control); /* erase */
  for (char axis = 0; axis < 2; axis ++) 
    drawPos[axis] = controlPos[axis];
  shape_pick(drawPos, color, control); /* draw */
}
