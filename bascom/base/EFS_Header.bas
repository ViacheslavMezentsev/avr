$nocompile


' EEPROM File System -> Version 1.0

Const EFS_OK = 0
Const EFS_ERROR = 1

' Types of entries.
Const EFS_ENTRY_FREE = $00
Const EFS_ENTRY_FILE = $01
Const EFS_ENTRY_DATA = $02

' EFS general constants.
Const EFS_TOTAL_ENTRIES	= $F0
Const EFS_FULL = $FF
Const EFS_EOF = $FE

'typedef struct _FILEINFO {
'
'	uint8_t entry;
'	uint8_t size;
'	uint8_t format;
'	uint8_t first_data_entry;
'
'} FILEINFO;

'typedef struct _ENTRIESCACHE {
'
'    uint8_t entries_cache[ 0xF0 ];
'    uint8_t first_free_entry;
'
'} ENTRIESCACHE;


Dim EFS_FirstFreeEntry As Byte
Dim EFS_EntriesCache( 254 ) As Byte


' Private.
Declare Function EFS_GetNextFreeEntry() As Byte
Declare Function EFS_AddDataEntry( ByVal ACurrentEntry As Byte ) As Byte

' Public.
Declare Sub EFS_Initialization
Declare Sub EFS_GetName( AFileInfo As Byte, ABuffer As Byte )
Declare Sub EFS_DeleteFile( AFileInfo As Byte )
Declare Function EFS_GetFileInfo( ByVal ANumber As Byte, ByVal AFormat As Byte, AFileInfo As Byte ) As Byte
Declare Function EFS_WriteFile( AFileInfo As Byte, ABuffer As Byte, ByVal APointer As Byte, ByVal ALength As Byte ) As Byte
Declare Function EFS_ReadFile( AFileInfo As Byte, ABuffer As Byte, ByVal APointer As Byte, ByVal ALength As Byte ) As Byte
Declare Function EFS_CreateFile( AFileInfo As Byte, ByVal AFormat As Byte, AName As String ) As Byte



