#ifndef _UI_H_
#define _UI_H_

// Display Northon Commander like function key menu on bottom of the screen. 
void drawfkeys( const char * fkeys[] );

// Display text-based selection menu on the screen.
// int runmenu(char x, char y, const char *menu[], int defaultitem);
uint8_t runmenu( uint8_t x, uint8_t y, const char * menu[], uint8_t defaultitem );

// Display empty frame on the screen.
void DrawFrame( uint8_t x, uint8_t y, uint8_t width, uint8_t height, EnColor color );

void FormMain();

#endif // _UI_H_