$nocompile


Const RTC_NONE = 0
Const RTC_DS1307 = 1
Const RTC_VS1307 = 2
Const RTC_PCF8523TS = 3
Const RTC_PCF8583 = 4

'address of ds1307
Const Ds1307w = &HD0
Const Ds1307r = &HD1


Dim RTCType As Byte
Dim CurrTime(7) As Byte


Declare Sub RTC_Initialization( ByVal AType As Byte )
Declare Sub RTC_GetDatetime( ADateTime As Byte )
Declare Sub RTC_PutDatetime( ADateTime As Byte )
Declare Sub RTC_ShowDate( ADateTime As Byte )
Declare Sub RTC_ShowTime( ADateTime As Byte )