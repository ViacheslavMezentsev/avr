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

#include "Defines.h"
#include "EFS.h"


struct{
    uint8_t entries_cache[0xf0];
    uint8_t first_free_entry;
} efs;


/* - Description: Initialize EFS
 *
 * - Flags: none
 */
void CEFS::Initialization() {
	
	uint8_t n;
	
	efs.first_free_entry = EFS_FULL;
	
	for ( n = 0; n < EFS_TOTAL_ENTRIES; n++ ) {

		efs.entries_cache[n] = eeprom_read_byte( ( uint8_t * ) ( n * 16 ) );

		if ( efs.first_free_entry == EFS_FULL && efs.entries_cache[n] == EFS_ENTRY_FREE ) efs.first_free_entry = n;
	}
	
}

/* - Description: Gets the next free entry on the EFS space
 *
 * - Flags: none
 */
uint8_t CEFS::GetNextFreeEntry() {
	
	uint8_t n;
	
	for ( n = efs.first_free_entry; n <EFS_TOTAL_ENTRIES; n++ ) {

		if ( efs.entries_cache[n] == EFS_ENTRY_FREE ) {

			efs.first_free_entry = efs.entries_cache[n];
			return n;
		}

	}
	
	return EFS_FULL;
	
}

/* - Description: Gets file information
 *
 * - Flags: 	number 	-> Number of the file (in the range of file that have the same format)
 * 				format	-> File format
 * 				*file 	-> Pointer where to store file information
 */
uint8_t CEFS::GetFileInfo( uint8_t number, uint8_t format, efs_file * file ) {
	
	uint8_t n, i, counter = 1;
	uint16_t address;
	
	for ( n = 0; n < EFS_TOTAL_ENTRIES; n++ ) {

		if ( efs.entries_cache[n] == EFS_ENTRY_FILE ) {

			address = n * 16;

			i = eeprom_read_byte( ( uint8_t * ) ( address + 1 ) );

			if ( i == format ) {

				if ( counter == number ) {

					//Get file info
					file->entry = n;
					file->format = i;
					file->size = eeprom_read_byte( ( uint8_t * ) ( address + 2 ) );
					file->first_data_entry = eeprom_read_byte( ( uint8_t * ) ( address + 15 ) );

					return 0;

				} else { 

					//Keep counting
					counter++;
				}

			}

		}

	}
	
	return 1;
	
}


/* - Description: Read file
 *
 * - Flags: 	*file 	-> File where to read
 * 				*buffer -> Buffer where to put the content read
 * 				length 	-> total bytes you want to read
 */
uint8_t CEFS::ReadFile( efs_file * file, uint8_t * buffer, uint8_t pointer, uint8_t length ) {
	
	uint8_t n;
	uint16_t address;
	
	/* Error if file length=0 or pointer is outside the file */
	if((!file->size) || (pointer>file->size)) return 1;
	
	/* Fix requested length if necessary */
	if((pointer+length) > file->size){
		length=file->size-pointer;
	}
	
	/* Switch to working entry */
	n=pointer/14;
	address=file->first_data_entry*16;
	while(n--){
		address=eeprom_read_byte((uint8_t*)(address+15))*16;
	}
	
	address+=pointer%14;
	
	/* Read data */
	while ( length ) {

		n = ( length < 14 ) ? length : ( 14 -pointer % 14 );

		eeprom_read_block(buffer, (const void*)(address+1), n);
		address=(eeprom_read_byte((uint8_t*)(address+n+1))*16);

		buffer += n;
		length -= n;
	}
	
	return 0;
	
}

/* - Description: Gets the name of a file
 *
 * - Flags: 	*file 	-> File where to take the name
 * 				*buffer -> Buffer where to store the name
 */
void CEFS::GetName( efs_file * file, char * buffer ) {
	
	eeprom_read_block( ( void * ) buffer, ( const void * ) ( file->entry * 16 + 3 ), 12 );
	
}


/* - Description: Adds a data entry to a file
 *
 * - Flags: 	current_entry -> Current entry in the file
 *
 */
uint8_t CEFS::AddDataEntry( uint8_t current_entry ) {
	
	uint8_t n;
	
	n = GetNextFreeEntry();

	if ( n == EFS_FULL) return n;
	
	eeprom_write_byte( ( uint8_t * ) ( current_entry * 16 + 15 ), n );
	eeprom_write_byte( ( uint8_t * ) ( n * 16 ), EFS_ENTRY_DATA );

	efs.entries_cache[n] = EFS_ENTRY_DATA;
	
	return n;
	
}

/* - Description: Writes content to a file
 *
 * - Flags: 	*file 	-> File where to write
 * 				*buffer -> Buffer where to take the data
 * 				length	-> Quantity of bytes you want to write
 */
uint8_t CEFS::WriteFile( efs_file *file, uint8_t *buffer, uint8_t pointer, uint8_t length){
	
	uint8_t current_entry, n;
		
	/* Error - pointer cannot be 'outside' the file */
	if(pointer > file->size) return 1;
	
	/* Switch to working entry */
	n=pointer/14;
	current_entry=file->first_data_entry;
	while(n--){
		current_entry=eeprom_read_byte((uint8_t*)(current_entry*16+15));
	}
	
	if(current_entry==0){
		//File needs the first data entry
		current_entry=AddDataEntry(file->entry);
		file->first_data_entry=current_entry;
	}
	
	/* Write file content */
	while(length){
		n=(length<14)?length:(14-pointer%14);
		eeprom_write_block((const void*)buffer, (void*)(current_entry*16+(pointer%14)+1), n);
		buffer+=n;
		pointer+=n;
		length-=n;
		if(pointer>file->size){
			if(length) current_entry=AddDataEntry(current_entry);
		}else{
			current_entry=eeprom_read_byte((uint8_t*)(current_entry*16+15));
		}
	}
	
	/* Mark last entry */
	eeprom_write_byte((uint8_t*)(current_entry*16+15), EFS_EOF);
	
	/* Update file size if necessary */
	if(pointer > file->size){
		eeprom_write_byte((uint8_t*)(file->entry*16+2), pointer);
		file->size=pointer;
	}
	
	return 0;
	
}


/* - Description: Deletes a file from EFS
 *
 * - Flags: 	*file 	-> File to delete
 *
 */
void CEFS::DeleteFile( efs_file * file ) {
	
	do {

		// Update first_free_entry if lower.
		if ( file->entry < efs.first_free_entry ) efs.first_free_entry = file->entry;

		// Mark entry as free & update cache.
		eeprom_write_byte( ( uint8_t * ) ( file->entry * 16 ), EFS_ENTRY_FREE );
		efs.entries_cache[ file->entry ] = EFS_ENTRY_FREE;

		// Get next entry.
		file->entry = eeprom_read_byte( ( uint8_t * ) ( file->entry * 16 + 15 ) );

	} while ( file->entry != EFS_EOF );
	
}


/* - Description: Creates a new file
 *
 * - Flags: 	*file 	-> Pointer where to store file information
 * 				format 	-> file format
 * 				*name 	-> File name (possible to be NULL)
 */
uint8_t CEFS::CreateFile(struct efs_file *file, uint8_t format, char *name){
	
	uint16_t address;
	
	/* Take a new free entry */
	file->entry=GetNextFreeEntry();
	if(file->entry==EFS_FULL) return 1;
	
	file->size=0;
	file->first_data_entry=0;
	file->format=format; //Format is only copied...!
	
	address=file->entry*16;

	/* Store file values */
	//New file entry
	eeprom_write_byte((uint8_t*)address, EFS_ENTRY_FILE);
	//File format
	eeprom_write_byte((uint8_t*)(address+1), format);
	//File length & first entry = 0
	eeprom_write_byte((uint8_t*)(address+2), 0);
	eeprom_write_byte((uint8_t*)(address+15), 0);
	//File name
	eeprom_write_block((const void*)name, (void*)(address+3), strlen(name)+1);
	
	/* Update cache */
	efs.entries_cache[file->entry]=EFS_ENTRY_FILE;
	
	return 0;
}
