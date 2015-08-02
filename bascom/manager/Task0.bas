$nocompile


Declare Sub Task0

Macro Task0_Initialization

$asm
   
' Указатель на контекст задачи находится в SRAM по адресу 0x0061.
' Размер 2 байта, по меньшему адресу младший байт.
	clr XH
	ldi XL, &H61
    
' Инициализируем указатель контекста задачи.
' В стеке задачи находятся: PC, регистры и статус.
	ldi Temp, &H04
	st X+, Temp
	ldi Temp, &H3C
	st X, Temp

' Переходим на стек задачи.
    ldi Temp, &H04
	out SPH, Temp
	ldi Temp, &H5F
	out SPL, Temp
    
' Начальное значение PC для задачи.
	ldi Temp, Low( Task0 )
	push Temp
	ldi Temp, High( Task0 )
	push Temp
       
' Инициализируем контекст нулями.
	clr ZeroReg
    push ZeroReg ' r0
    push ZeroReg ' r1
    push ZeroReg ' r2
    push ZeroReg ' r3
    
    ldi Temp, &H00
    push Temp ' r4
    ldi Temp, &H03
    push Temp ' r5
    
    push ZeroReg ' r6
    push ZeroReg ' r7
    push ZeroReg ' r8
    push ZeroReg ' r9
    push ZeroReg ' r10
    push ZeroReg ' r11
    push ZeroReg ' r12
    push ZeroReg ' r13
    push ZeroReg ' r14
    push ZeroReg ' r15
    push ZeroReg ' r16
    push ZeroReg ' r17
    push ZeroReg ' r18
    push ZeroReg ' r19
    push ZeroReg ' r20
    push ZeroReg ' r21
    push ZeroReg ' r22
    push ZeroReg ' r23
    push ZeroReg ' r24
    push ZeroReg ' r25
    push ZeroReg ' r26
    push ZeroReg ' r27
    
    ldi Temp, &HE0
    push Temp ' r28
    ldi Temp, &H03
    push Temp ' r29
    
    push ZeroReg ' r30
    push ZeroReg ' r31
    
' Начальное значение статус регистра.
	push ZeroReg
    
$end asm

End Macro


Sub Task0

    Local I As Byte, Ch As Byte
    
    I = 0
    
    Do
    
        Waitms 500
        Incr I
        
        ' Блок защиты от переключения.
        cli
        
        Console_SetForegroundColor clWhite
        
        Ch = Asc( "1" )
        PrintBin Ch
        
        Ch = Asc( "-" )
        PrintBin Ch
        
        ' Десятки.
        Ch = I \ 10
        Ch = Ch + &H30
        PrintBin Ch

        ' Единицы.
        Ch = I mod 10
        Ch = Ch + &H30
        PrintBin Ch

        Print
        
        sei
        
    Loop

End Sub