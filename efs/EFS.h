/*  openplayer firmware - openplayer, an open source portable music player
    Copyright (C) 2008  Gerard Marull Paretas - <gerardmarull[at]gmail[dot]com>

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

/**************************************************************************
 * 
 * EEPROM File System -> Version 1.0
 * 
 * File type: Source
 * File name: efs.c
 * 
 **************************************************************************/

#ifndef EFS_H_
#define EFS_H_

/* Types of entries */
#define EFS_ENTRY_FREE	0x00
#define EFS_ENTRY_FILE	0x01
#define EFS_ENTRY_DATA	0x02

/* EFS general constants */
#define EFS_TOTAL_ENTRIES	0xf0
#define EFS_FULL			0xff
#define EFS_EOF				0xfe

struct efs_file{
	uint8_t entry;
	uint8_t size;
	uint8_t format;
	uint8_t first_data_entry;
};

/***********************/
/* Класс CEFS */
/***********************/
class CEFS {

private:

    static uint8_t GetNextFreeEntry();    
    static uint8_t AddDataEntry( uint8_t current_entry );

public:
    
    static void Initialization();
    static void GetName( efs_file * file, char * buffer);
    static uint8_t GetFileInfo( uint8_t number, uint8_t format, efs_file * file);
    static uint8_t WriteFile( efs_file * file, uint8_t * buffer, uint8_t pointer, uint8_t length );
    static uint8_t ReadFile( efs_file * file, uint8_t * buffer, uint8_t pointer, uint8_t length );
    static uint8_t CreateFile( efs_file * file, uint8_t format, char * name );
    static void DeleteFile( efs_file * file);   

};


#endif /*EFS_H_*/
