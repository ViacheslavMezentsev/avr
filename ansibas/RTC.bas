$nocompile


Sub RTC_Initialization( ByVal AType As Byte )

    RTCType = AType

End Sub


Sub RTC_GetDatetime( ADateTime As Byte )

    Local Minu As Byte
    Local Hour As Byte
    Local Datt As Byte
    Local Mont As Byte
    Local Year As Byte
    Local I As Byte
    Local J As Byte

    if RTCType = RTC_NONE then

        Exit Sub

    elseif RTCType = RTC_DS1307 Or RTCType = RTC_VS1307 then

        I2cstart
        I2cwbyte Ds1307w
        I2cwbyte 0

        I2cstart
        I2cwbyte Ds1307r
        I2crbyte ADateTime(1), Ack
        I2crbyte ADateTime(2), Ack
        I2crbyte ADateTime(3), Ack
        I2crbyte ADateTime(4), Ack
        I2crbyte ADateTime(5), Ack
        I2crbyte ADateTime(6), Ack
        I2crbyte ADateTime(7), Nack
        I2cstop

    elseif RTCType = RTC_PCF8583 then

    End If

End Sub


Sub RTC_PutDatetime( ADateTime As Byte )

End Sub


Sub RTC_ShowDate( ADateTime As Byte )

    Local Ch As Byte
    Local AYear As Byte, AMonth As Byte, ADay As Byte

    ADay = ADateTime(5)
    ADay = MakeDec( ADay )

    AMonth = ADateTime(6)
    AMonth = MakeDec( AMonth )

    AYear = ADateTime(7)
    AYear = MakeDec( AYear )

    ' ����� ����.
    ' �������.
    Ch = ADay \ 10
    Ch = Ch + &H30
    PrintBin Ch

    ' �������.
    Ch = ADay mod 10
    Ch = Ch + &H30
    PrintBin Ch

    Ch = Asc( "-" )
    PrintBin Ch

    ' ����� ������.
    ' �������.
    Ch = AMonth \ 10
    Ch = Ch + &H30
    PrintBin Ch

    ' �������.
    Ch = AMonth mod 10
    Ch = Ch + &H30
    PrintBin Ch

    Ch = Asc( "-" )
    PrintBin Ch

    ' ����� ����.
    ' �������.
    Ch = AYear \ 10
    Ch = Ch + &H30
    PrintBin Ch

    ' �������.
    Ch = AYear mod 10
    Ch = Ch + &H30
    PrintBin Ch

End Sub


Sub RTC_ShowTime( ADateTime As Byte )

    Local Ch As Byte
    Local AHour As Byte, AMinutes As Byte, ASeconds As Byte

    ASeconds = ADateTime(1)
    ASeconds = MakeDec( ASeconds )

    AMinutes = ADateTime(2)
    AMinutes = MakeDec( AMinutes )

    AHour = ADateTime(3)
    AHour = MakeDec( AHour )

    ' ����� �����.
    ' �������.
    Ch = AHour \ 10
    Ch = Ch + &H30
    PrintBin Ch

    ' �������.
    Ch = AHour mod 10
    Ch = Ch + &H30
    PrintBin Ch

    Ch = Asc( ":" )
    PrintBin Ch

    ' ����� �����.
    ' �������.
    Ch = AMinutes \ 10
    Ch = Ch + &H30
    PrintBin Ch

    ' �������.
    Ch = AMinutes mod 10
    Ch = Ch + &H30
    PrintBin Ch

    Ch = Asc( ":" )
    PrintBin Ch

    ' ����� ������.
    ' �������.
    Ch = ASeconds \ 10
    Ch = Ch + &H30
    PrintBin Ch

    ' �������.
    Ch = ASeconds mod 10
    Ch = Ch + &H30
    PrintBin Ch

End Sub