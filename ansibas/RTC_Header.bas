$nocompile


'address of ds1307
Const Ds1307w = &HD0
Const Ds1307r = &HD1


Declare Sub RTC_Initialization
Declare Sub RTC_GetDatetime( ATime As Byte )
Declare Sub RTC_ShowDate( ATime As Byte )
Declare Sub RTC_ShowTime( ATime As Byte )