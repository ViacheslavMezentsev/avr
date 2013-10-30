$nocompile

' /****************************
' *  К О Н Ф И Г У Р А Ц И Я
' *  ~~~~~~~~~~~~~~~~~~~~~~~
' ****************************/

' Integer and Word constants must end with the %-sign.
' Long constants must end with the &-sign.
' Single constants must end with the !-sign.
' Double constants must end with the #-sign.

$regfile = "m16def.dat"
$crystal = 8000000
$hwstack = 40
$swstack = 16
$framesize = 64
'$sim

' ‘корость работы внутреннего USART
'$baud = 9600
'$baud = 19200
'$baud = 38400
$baud = 57600
'$baud = 115200

' we do not use software emulated I2C but the TWI
$lib "i2c_twi.lbx"

Config Scl = PORTC.0
Config Sda = PORTC.1

' wanted clock frequency when using $lib "i2c_twi.lbx"
Config Twi = 100000


RTC_DDR_PCF8583_CS alias DDRC.DDC2
RTC_PORT_PCF8583_CS alias PORTC.PC2