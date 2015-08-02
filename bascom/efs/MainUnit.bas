'*******************************************************************************
'* Автор: Мезенцев Вячеслав Николаевич
'* Электропочта: viacheslavmezentsev@gmail.com
'* skype: viacheslavmezentsev
'* icq: 10333578
'*
'* Проект: EEPROM File System.
'* Дата:  08.11.2013 г.
'* Компилятор: BASCOM-AVR 2.0.7.5
'* MCU:  ATmega16
'* Тактовая частота:  8000000  Hz
'*******************************************************************************

' Директивы компилятора.

$regfile = "m16def.dat"
$crystal = 8000000
$hwstack = 40
$swstack = 64
$framesize = 24
'$map
'$sim
'$noinit

' Скорость работы внутреннего USART
' 9600, 19200, 38400, 57600, 115200
'$baud = 38400


' -=[ Переменные в ОЗУ ]=-

Dim Result As Byte
Dim AFile(4) As Byte
Dim szName As String * 13
Dim pData(12) As Byte At szName Overlay


' Точка входа в основную программу.
Declare Sub Main : Main : End


' /***********************  
' *  Д Е К Л А Р А Ц И Я    
' *  ~~~~~~~~~~~~~~~~~~~    
' ***********************/  

' Подключаем заголовочные файлы модулей.

$include "..\base\EFS_Header.bas"


' /***********************
' *  Р Е А Л И З А Ц И Я
' *  ~~~~~~~~~~~~~~~~~~~
' ************************/

' Подключаем модули.

$include "..\base\EFS.bas"


Sub Main

    Local n As Byte, Temp As Byte

    ' Заполняем часть EEPROM нулями.
    For n = 0 To 239
    
        Temp = &H00
        WriteEeprom Temp, n
        
    Next       
    
    EFS_Initialization
    
    szName = "MyFile"    
    Result = EFS_CreateFile( AFile(1), &HCF, szName )
    
    szName = "-123456-"
    Result = EFS_WriteFile( AFile(1), pData(1), 0, 8 )
    
    Do : Loop

End Sub

