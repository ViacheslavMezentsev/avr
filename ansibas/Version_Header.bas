$nocompile

Declare Function Version_GetMajor() As DWORD
Declare Function Version_GetMinor() As DWORD
Declare Function Version_GetBuild() As DWORD
Declare Function Version_GetRevision() As DWORD
Declare Function Version_GetVersionString() As String
Declare Function Version_GetBuildDateString() As String

Function Version_GetMajor() As DWORD : Version_GetMajor = 0 : End Function
Function Version_GetMinor() As DWORD : Version_GetMinor = 1 : End Function
Function Version_GetBuild() As DWORD : Version_GetBuild = 5049 : End Function
Function Version_GetRevision() As DWORD : Version_GetRevision = 26593 : End Function
Function Version_GetVersionString() As String : Version_GetVersionString = "0.1.5049.26593" : End Function
Function Version_GetBuildDateString() As String : Version_GetBuildDateString = "29 окт€бр€ 2013 г." : End Function