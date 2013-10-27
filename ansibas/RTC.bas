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

    ' Вывод дней.
    ' Десятки.
    Ch = ADay \ 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    ' Единицы.
    Ch = ADay mod 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    Ch = Asc( "-" )
    Console_PutChar Ch, cp866

    ' Вывод месяца.
    ' Десятки.
    Ch = AMonth \ 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    ' Единицы.
    Ch = AMonth mod 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    Ch = Asc( "-" )
    Console_PutChar Ch, cp866

    ' Вывод года.
    ' Десятки.
    Ch = AYear \ 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    ' Единицы.
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

    ' Вывод часов.
    ' Десятки.
    Ch = AHour \ 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    ' Единицы.
    Ch = AHour mod 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    Ch = Asc( ":" )
    Console_PutChar Ch, cp866

    ' Вывод минут.
    ' Десятки.
    Ch = AMinutes \ 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    ' Единицы.
    Ch = AMinutes mod 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    Ch = Asc( ":" )
    Console_PutChar Ch, cp866

    ' Вывод секунд.
    ' Десятки.
    Ch = ASeconds \ 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    ' Единицы.
    Ch = ASeconds mod 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

End Sub