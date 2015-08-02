$nocompile

' Модуль: Version_Header.bas
' Создан: 15 февраля 2015 г. 19:06:55
' Автор: Мезенцев В. Н.
'
' Модуль для автоматизированной работы с версией программы.
'
' Внимание. Этот файл формируется автоматически. Любые внесённые
' здесь изменения пропадут перед компиляцией.

'Const VERSION_ENABLED = 0 ' +/- ... байт(а)

#if VERSION_ENABLED

Declare Function Version_GetMajor() As DWORD
Declare Function Version_GetMinor() As DWORD
Declare Function Version_GetBuild() As DWORD
Declare Function Version_GetRevision() As DWORD
Declare Function Version_GetVersionString() As String
Declare Function Version_GetBuildDateString() As String

Function Version_GetMajor() As DWORD : Version_GetMajor = 0 : End Function
Function Version_GetMinor() As DWORD : Version_GetMinor = 1 : End Function
Function Version_GetBuild() As DWORD : Version_GetBuild = 5524 : End Function
Function Version_GetRevision() As DWORD : Version_GetRevision = 34407 : End Function
Function Version_GetVersionString() As String : Version_GetVersionString = "0.1.5524.34407" : End Function
Function Version_GetBuildDateString() As String : Version_GetBuildDateString = "15 февраля 2015 г." : End Function

#endif
