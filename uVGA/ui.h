#ifndef _UI_H_
#define _UI_H_

/*
    ui.h
    User interface functions.
    Part of MicroVGA CONIO library / demo project
    Copyright (c) 2008-9 SECONS s.r.o., http://www.MicroVGA.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//Display Northon Commander like function key menu on bottom of the screen. 
void drawfkeys( const char * fkeys[] );

//Display text-based selection menu on the screen.
//int runmenu(char x, char y, const char *menu[], int defaultitem);
uint8_t runmenu( uint8_t x, uint8_t y, const char * menu[], uint8_t defaultitem );

//Display empty frame on the screen.
void drawframe( uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color );

void UIMain();

#endif // _UI_H_