#ifndef buzzer_included
#define buzzer_included

void buzzer_init();
void buzzer_set_period(short cycles);
void play_song(short cycles);
void short_play(short cycles);
void lazer(char mult);
void play_lazer();
void buzzer_off();
void buzzer_on();

#endif // included
