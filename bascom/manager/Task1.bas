$nocompile

Declare Sub Task1

Macro Task1_Initialization

$asm
   
' Указатель на контекст задачи находится в SRAM по адресу 0x0063.
' Размер 2 байта, по меньшему адресу младший байт.
	clr XH
	ldi XL, &H63
    
' Инициализируем указатель контекста задачи.
' В стеке задачи находятся: PC, регистры и статус.
	ldi Temp, &H02
	st X+, Temp
	ldi Temp, &HBC
	st X, Temp

' Переходим на стек задачи.
    ldi Temp, &H02
	out SPH, Temp
	ldi Temp, &HDF
	out SPL, Temp
    
' Начальное значение PC для задачи.
	ldi Temp, Low( Task1 )
	push Temp
	ldi Temp, High( Task1 )
	push Temp
       
' Инициализируем контекст нулями.
	clr ZeroReg
    push ZeroReg ' r0
    push ZeroReg ' r1
    push ZeroReg ' r2
    push ZeroReg ' r3
    
    ldi Temp, &H80
    push Temp ' r4
    ldi Temp, &H01
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
    
    ldi Temp, &H60
    push Temp ' r28
    ldi Temp, &H02
    push Temp ' r29
    
    push ZeroReg ' r30
    push ZeroReg ' r31
    
' Начальное значение статус регистра.
	push ZeroReg
    
$end asm

End Macro


Sub Task1

    Local I As Byte, Ch As Byte
    
    I = 0
    
    Do
    
        Waitms 1000
        Incr I
        
        ' Блок защиты от переключения.
        cli
        
        Console_SetForegroundColor clMagenta
        
        Ch = Asc( "2" )
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