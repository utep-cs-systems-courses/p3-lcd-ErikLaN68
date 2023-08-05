#include "machines.h"

//takes the state and uses switch to goto state
void
state_switch(char state)
{
  switch (state) {
  case 1:
    state_1();
    state = 0;
    break;
  case 2:
    state_2();
    state = 0;
    break;
  case 3:
    state_3();
    state = 0;
    break;
  default:
    state = 0;
    break;
  }
}
