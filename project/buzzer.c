#include <msp430.h>
#include "libTimer.h"

void
buzzer_init()
{
    /* 
       Direct timer A output "TA0.1" to P2.6.  
        According to table 21 from data sheet:
          P2SEL2.6, P2SEL2.7, anmd P2SEL.7 must be zero
          P2SEL.6 must be 1
        Also: P2.6 direction must be output
    */
    timerAUpmode();		/* used to drive speaker */
    P2SEL2 &= ~(BIT6 | BIT7);
    P2SEL &= ~BIT7; 
    P2SEL |= BIT6;
    P2DIR = BIT6;		/* enable output to speaker (P2.6) */
}

void
buzzer_set_period(short cycles) /* buzzer clock = 2MHz.  (period of 1k results in 2kHz tone) */
{
  //timerAUpmode();
  CCR0 = cycles; 
  CCR1 = cycles >> 1;		/* one half cycle */
}

//Needed because when button is just pressed it doesn't leave enough time to play sound
//Plays sound at given cycles number
void
play_song(short cycles)
{
  short count = 2000;
  //Runs while count greater than 2000
  while (count > 0) {
    buzzer_set_period(cycles);
    count--;
  }
}

//Takes given cycles and plays song for short amount of time
void
short_play(short cycles)
{
  short count = 100;
  //Runs while count greater than 100
  while (count > 0) {
    buzzer_set_period(cycles);
    count--;
  }
}

//Plays a lazer like sound and can be changed with the variable mult
void
lazer(char mult)
{
  play_song(4545 * mult);
  play_song(6079 * mult);
  play_song(4545 * mult);
  play_song(4545 * mult);
  play_song(6079 * mult);
  play_song(4545 * mult);
}

//Turns buzzer off
void
buzzer_off()
{
  CCR0 = 0;
  CCR1 = 0;
}

//Turns buzzer on
void
buzzer_on()
{
  P2DIR = BIT6;
}
