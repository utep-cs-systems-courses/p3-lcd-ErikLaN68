#include "lcdutils.h"
#include "lcddraw.h"

void
draw_ship(int controlCol, int controlRow, unsigned short color)
{
  //fixed car size
  char size = 21;
  //Runs the for loop the correct amount of times
  char stop = (size/2) + 1;
  char count = 0;
  //How many layers are made
  while (count < stop) {
    //Runs till size to make the layers
    for (int i = 0; i < size; i++) {
      //only have to add to controlCol to move along
      drawPixel(controlCol+i,controlRow,color);
    }
    //moving the position up and decreasing the size
    size -= 2;
    controlCol += 1;
    controlRow -= 1;
    count++;
  }
}
