$nocompile


Sub RTC_GetDatetime( ATime As Byte )

  I2cstart
  I2cwbyte Ds1307w
  I2cwbyte 0

  I2cstart
  I2cwbyte Ds1307r
  I2crbyte ATime(1), Ack
  I2crbyte ATime(2), Ack
  I2crbyte ATime(3), Ack
  I2crbyte ATime(4), Ack
  I2crbyte ATime(5), Ack
  I2crbyte ATime(6), Ack
  I2crbyte ATime(7), Nack
  I2cstop

End Sub


Sub RTC_ShowDate( ATime As Byte )

    Local Ch As Byte
    Local AYear As Byte, AMonth As Byte, ADay As Byte

    ADay = ATime(5)
    ADay = MakeDec(ADay)

    AMonth = ATime(6)
    AMonth = MakeDec(AMonth)

    AYear = ATime(7)
    AYear = MakeDec(AYear)

    ' ����� ����.
    ' �������.
    Ch = ADay \ 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    ' �������.
    Ch = ADay mod 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    Ch = Asc( "-" )
    Console_PutChar Ch, cp866

    ' ����� ������.
    ' �������.
    Ch = AMonth \ 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    ' �������.
    Ch = AMonth mod 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    Ch = Asc( "-" )
    Console_PutChar Ch, cp866

    ' ����� ����.
    ' �������.
    Ch = AYear \ 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    ' �������.
    Ch = AYear mod 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

End Sub


Sub RTC_ShowTime( ATime As Byte )

    Local Ch As Byte
    Local AHour As Byte, AMinutes As Byte, ASeconds As Byte

    ASeconds = ATime(1)
    ASeconds = MakeDec(ASeconds)

    AMinutes = ATime(2)
    AMinutes = MakeDec(AMinutes)

    AHour = ATime(3)
    AHour = MakeDec(AHour)

    ' ����� �����.
    ' �������.
    Ch = AHour \ 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    ' �������.
    Ch = AHour mod 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    Ch = Asc( ":" )
    Console_PutChar Ch, cp866

    ' ����� �����.
    ' �������.
    Ch = AMinutes \ 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    ' �������.
    Ch = AMinutes mod 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    Ch = Asc( ":" )
    Console_PutChar Ch, cp866

    ' ����� ������.
    ' �������.
    Ch = ASeconds \ 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    ' �������.
    Ch = ASeconds mod 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

End Sub