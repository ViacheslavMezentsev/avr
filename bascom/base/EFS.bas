$nocompile


' Description: Gets the next free entry on the EFS space.
' Flags: none.
Function EFS_GetNextFreeEntry() As Byte

    Local n As Byte, Temp As Byte

    n = EFS_FirstFreeEntry
    
    While n < EFS_TOTAL_ENTRIES
    
		If EFS_EntriesCache( n + 1 ) = EFS_ENTRY_FREE Then

			EFS_FirstFreeEntry = EFS_EntriesCache( n + 1 )
			
            EFS_GetNextFreeEntry = n
            
            Exit Function
            
		End If
        
        Incr n
    
    WEnd
       
    EFS_GetNextFreeEntry = EFS_FULL
    
End Function


' Description: Adds a data entry to a file.
' Flags: 	ACurrentEntry -> Current entry in the file.
Function EFS_AddDataEntry( ByVal ACurrentEntry As Byte ) As Byte

	Local n As Byte
    Local Addr As Word
	
    n = EFS_GetNextFreeEntry()

	If n = EFS_FULL Then
    
        EFS_AddDataEntry = n
        
        Exit Function
    
    End If
	   
    Addr = ACurrentEntry
    Shift Addr, Left, 4
    Addr = Addr + 15
    
    WriteEeprom n, Addr
    
    Addr = n
    Shift Addr, Left, 4   
    
    EFS_EntriesCache( n + 1 ) = EFS_ENTRY_DATA	
    EFS_AddDataEntry = n
    
    n = EFS_ENTRY_DATA
    WriteEeprom n, Addr	
    
End Function


' Description: Initialize EFS.
' Flags: none.
Sub EFS_Initialization

    Local n As Byte, Temp As Byte
    Local Addr As Word
    
    EFS_FirstFreeEntry = EFS_FULL
    
    For n = 1 To EFS_TOTAL_ENTRIES
    
        Addr = n
        Decr Addr
        Shift Addr, Left, 4       
        
        ReadEeprom Temp, Addr
        
        EFS_EntriesCache(n) = Temp
        
        If EFS_FirstFreeEntry = EFS_FULL And Temp = EFS_ENTRY_FREE Then
        
            EFS_FirstFreeEntry = n - 1
        
        End If
    
    Next
    
End Sub


' Description: Gets the name of a file.
'
' Flags: 	AFileInfo 	-> File where to take the name;
'			ABuffer     -> Buffer where to store the name.
Sub EFS_GetName( AFileInfo As Byte, ABuffer As Byte )

    Local I As Byte
    Local Addr As Word
    
    Addr = AFileInfo(1)    
    Shift Addr, Left, 16    
    Addr = Addr + 3
    
    For I = 1 To 12
    
        ReadEeprom ABuffer(I), Addr
    
    Next   

End Sub


' Description: Deletes a file from EFS.
' Flags: AFileInfo 	-> File to delete.
Sub EFS_DeleteFile( AFileInfo As Byte )

    Local FileEntry As Byte
    Local Addr As Word
    
    FileEntry = AFileInfo(1)
    
	Do

		' Update EFS_FirstFreeEntry if lower.
		If FileEntry < EFS_FirstFreeEntry Then EFS_FirstFreeEntry = FileEntry

		' Mark entry as free & update cache.
        Addr = FileEntry
        Shift Addr, Left, 4        
        
		EFS_EntriesCache( FileEntry + 1 ) = EFS_ENTRY_FREE  
        WriteEeprom EFS_EntriesCache( FileEntry + 1 ), Addr

		' Get next entry.
        Addr = Addr + 15       
        ReadEeprom FileEntry, Addr

	Loop Until FileEntry <> EFS_EOF
    
End Sub


' Description: Gets file information.
'
' Flags: 	ANumber 	-> Number of the file (in the range of file that have the same format);
'			AFormat	    -> File format;
'			AFileInfo 	-> Pointer where to store file information.
Function EFS_GetFileInfo( ANumber As Byte, ByVal AFormat As Byte, AFileInfo As Byte ) As Byte

	Local n As Byte, I As Byte, Counter As Byte
	Local Addr As Word

    Counter = 1
    
    For n = 1 To EFS_TOTAL_ENTRIES
    
        If EFS_EntriesCache(n) = EFS_ENTRY_FILE Then
        
            Addr = n
            Decr Addr
            Shift Addr, Left, 4            
            
            ReadEeprom I, Addr
            
            If I = AFormat Then
            
                If Counter = ANumber Then
                
					' Get file info.
					AFileInfo(1) = n
					AFileInfo(3) = I
                    
                    Incr Addr
                    ReadEeprom AFileInfo(2), Addr
					
                    Addr = Addr + 13
                    ReadEeprom AFileInfo(4), Addr
                    
                    EFS_GetFileInfo = EFS_OK
                    
                    Exit Function
                    
                Else

					' Keep counting.
					Incr Counter

                End If
            
            End If
            
        End If
    
    Next
    
    EFS_GetFileInfo = EFS_ERROR

End Function


' Description: Writes content to a file.
'
' Flags: 	AFileInfo 	-> File where to write;
'			ABuffer     -> Buffer where to take the data;
'			ALength	    -> Quantity of bytes you want to write.
Function EFS_WriteFile( AFileInfo As Byte, ABuffer As Byte, ByVal APointer As Byte, ByVal ALength As Byte ) As Byte

	Local CurrentEntry As Byte, n As Byte, I As Byte, Temp As Byte
    Local Addr As Word
    
	' Error - pointer cannot be 'outside' the file.
	If APointer > AFileInfo(1) Then
    
        EFS_WriteFile = EFS_ERROR
        
        Exit Function
        
    End If

	' Switch to working entry.
	n = APointer \ 14
	CurrentEntry = AFileInfo(4)

	While n > 0
		
        Addr = CurrentEntry
        Shift Addr, Left, 4
        Addr = Addr + 15
        
        ReadEeprom CurrentEntry. Addr
               
        Decr n
        
	WEnd

	If CurrentEntry = 0 Then

		' File needs the first data entry.
		CurrentEntry = EFS_AddDataEntry( AFileInfo(1) )
        
		AFileInfo(4) = CurrentEntry
        
	End If
    
	' Write file content.
	While ALength > 0

        If ALength < 14 Then
        
            n = ALength
            
        Else
        
            Addr = APointer Mod 14
            n = 14 - Addr
            
        End If	

        For I = 1 To n
        
            Addr = CurrentEntry
            Shift Addr, Left, 4
            
            Temp = APointer Mod 14
                        
            Addr = Addr + Temp
            Addr = Addr + I
            
            Temp = ABuffer(I)
            WriteEeprom Temp, Addr
            
        Next

		APointer = APointer + n
		ALength = ALength - n

		If APointer > AFileInfo(1) Then

			If ALength > 0 Then CurrentEntry = EFS_AddDataEntry( CurrentEntry )

		Else

            Addr = CurrentEntry
            Shift Addr, Left, 4
            Addr = Addr + 15
            
            ReadEeprom CurrentEntry, Addr            
            
		End If

	WEnd

	' Mark last entry.
    Addr = CurrentEntry
    Shift Addr, Left, 4
    Addr = Addr + 15
    
    Temp = EFS_EOF
    WriteEeprom Temp, Addr
    
	' Update file size if necessary.
	If APointer > AFileInfo(1) Then

		Addr = AFileInfo(1)
        Shift Addr, Left, 4
        Addr = Addr + 2
        
        WriteEeprom APointer, Addr
        
		AFileInfo(2) = APointer
        
	End If
	   
    EFS_WriteFile = EFS_OK

End Function


' Description: Read file.
'
' Flags: 	AFileInfo 	-> File where to read;
'			ABuffer     -> Buffer where to put the content read;
'			ALength 	-> total bytes you want to read.
Function EFS_ReadFile( AFileInfo As Byte, ABuffer As Byte, ByVal APointer As Byte, ByVal ALength As Byte ) As Byte

	Local n As Byte, I As Byte, Temp As Byte
	Local Addr As Word

	' Error if file length = 0 or pointer is outside the file.
	If AFileInfo(2) = 0 Or APointer > AFileInfo(2) Then
    
        EFS_ReadFile = EFS_ERROR
        
        Exit Function
    
    End If

	' Fix requested length if necessary.
    Temp = APointer + ALength
    
	If Temp > AFileInfo(2) Then
    
        ALength = AFileInfo(2)		
        ALength = ALength - APointer
        
	End If

	' Switch to working entry.
	n = APointer \ 14
	Addr = AFileInfo(4)
    Shift Addr, Left, 4
    
	While n > 0

        Addr = Addr + 15
        ReadEeprom Temp, Addr
		
        Addr = Temp
        Shift Addr, Left, 4
        
        Decr n
        
	WEnd

    Temp = APointer Mod 14
    Addr = Addr + Temp
    
	' Read data.
	While ALength > 0

        If ALength < 14 Then
        
            n = ALength
            
        Else
        
            Addr = APointer
            Addr = Addr Mod 14
            n = 14 - Addr
            
        End If        

        '_EEGETBLOCK( buffer, address + 1, n );
        For I = 1 To n
                   
            'ReadEeprom ABuffer(I), Addr
            
        Next
        
		Addr = Addr + n
        Incr Addr
        
        ReadEeprom Temp, Addr
        Addr = Temp

		'buffer += n;
		ALength = ALength - n
        
	WEnd
    
    EFS_ReadFile = EFS_OK

End Function


' Description: Creates a new file.
'
' Flags: 	AFileInfo 	-> Pointer where to store file information;
'			AFormat 	-> file format;
'			AName 	    -> File name (possible to be NULL).
Function EFS_CreateFile( AFileInfo As Byte, ByVal AFormat As Byte, AName As String ) As Byte

    Local I As Byte, Temp As Byte
    Local Addr As Word
    
	' Take a new free entry.
	AFileInfo(1) = EFS_GetNextFreeEntry()

	If AFileInfo(1) = EFS_FULL Then 
    
        EFS_CreateFile = EFS_ERROR
        
        Exit Function

    End If

    ' File size.
	AFileInfo(2) = 0
    
    ' File format.
	AFileInfo(3) = AFormat

    ' File first data entry.
	AFileInfo(4) = 0   

    Addr = AFileInfo(1)
    Shift Addr, Left, 4
    
	' Store file values.
	' New file entry.
    Temp = EFS_ENTRY_FILE
    WriteEeprom Temp, Addr
    
	' File format.
    Incr Addr
	WriteEeprom AFormat, Addr

	' File length & first entry = 0.
    Incr Addr
    Temp = 0
	WriteEeprom Temp, Addr
    
    Addr = Addr + 13
	WriteEeprom Temp, Addr
    
    ' File name.
    Addr = AFileInfo(1) + 3
    WriteEeprom AName(1), Addr
    
    ' Завершающий ноль.
    Temp = Len( AName )
    Addr = Addr + Temp
    Incr Addr
    Temp = 0
    WriteEeprom Temp, Addr

	' Update cache.
	EFS_EntriesCache( AFileInfo(1) + 1 ) = EFS_ENTRY_FILE

    EFS_CreateFile = EFS_OK

End Function
