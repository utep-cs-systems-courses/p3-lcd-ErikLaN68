#ifndef background_included
#define background_included

void draw_shape_1(int col, int row, unsigned short color);
void draw_shape_2(int col, int row, unsigned short color);
void start_page();
void end_page();
void lost_page();
void screen_update_shape(short controlPos[], short drawPos[], unsigned short color, char control);

#endif
