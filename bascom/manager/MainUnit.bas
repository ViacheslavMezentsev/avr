'*******************************************************************************
'* Автор: Мезенцев Вячеслав Николаевич
'* Электропочта: viacheslavmezentsev@gmail.com
'* skype: viacheslavmezentsev
'* icq: 10333578
'*
'* Проект: Переключатель задач.
'* Дата начала проекта:  06.11.2013 г.
'* Компилятор: BASCOM-AVR 2.0.7.5
'* MCU:  ATmega16
'* Тактовая частота:  8000000  Hz
'*******************************************************************************

' Регистры, используемые Bascom:
' R4 and R5 are used to point to the stack frame or the temp data storage,
' R6 is used to store some bit variables,
' R8 and R9 are used as a data pointer for the READ statement.


' Директивы компилятора.

$regfile = "m16def.dat"
$crystal = 8000000
$hwstack = 128
$swstack = 128
$framesize = 120
'$noramclear
'$map
'$sim
'$noinit


' Прерывания.
on Timer0 TIM0_OVF nosave

' Настройка Таймера 0.
Config Timer0 = Timer, Prescale = 64

' Скорость работы внутреннего USART
' 9600, 19200, 38400, 57600, 115200
$baud = 38400


' -=[ Переменные в ОЗУ ]=-
Dim Array(5) As Byte At &H60
Dim Task0Addr As Word At &H61 overlay
Dim Task1Addr As Word At &H63 overlay

Dim Var1 As Byte
Dim Var2 As Byte


' Точка входа в основную программу.
Declare Sub Main : Main : End


' /***********************  
' *  Д Е К Л А Р А Ц И Я    
' *  ~~~~~~~~~~~~~~~~~~~    
' ***********************/  

' Подключаем заголовочные файлы модулей.

$include "Config_Header.bas"
$include "../base/Console_Header.bas"
$include "../base/CommandShell_Header.bas"


' /***********************
' *  Р Е А Л И З А Ц И Я
' *  ~~~~~~~~~~~~~~~~~~~
' ************************/

' Подключаем модули.

$include "../base/Console.bas"
$include "../base/CommandShell.bas"
$include "Task0.bas"
$include "Task1.bas"


Sub Main

    ' Вывод приглашения командной строки.
    CommandShell_Prompt
    Print
    
    Task0_Initialization
    Task1_Initialization
    
$asm

.def Temp = r16
    
' Начальное значение счётчика нулевого таймера.
	clr Temp
	out TCNT0, Temp

' Переход на нулевую задачу.
    ldi Temp, &H04
	out SPH, Temp
	ldi Temp, &H5F
	out SPL, Temp
    
' Начальное значение PC для  нулевой задачи.
	ldi Temp, Low( Task0 )
	push Temp
	ldi Temp, High( Task0 )
	push Temp

' Инициализируем указатель на область фреймов.    
    ldi Temp, &H00
    mov r4, Temp
    ldi Temp, &H03
    mov r5, Temp

' Инициализируем указатель на область адресов переменных.    
    ldi Temp, &HE0
    mov r28, Temp
    ldi Temp, &H03
    mov r29, Temp
    
    clr Temp
    
$end asm

    Enable Timer0
    Goto TIM0_OVF

End Sub


' Обработчик прерывания от нулевого таймера.
TIM0_OVF:

$asm

' Младшие регистры.
.def	ZeroReg		=	r11

' Старшие регистры.
.def	Temp		=	r16
.def	TaskCnt		=	r17

.equ    COUNT_MASK  =   &H01

' В стеке находятся два байта PC. Они указывают на место
' прерывания текущей задачи.

' Сохраняем регистры общего назначения.
    push r0
    push r1
    push r2
    push r3
    push r4
    push r5
    push r6
    push r7
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
	push r16
	push r17
	push r18
	push r19
	push r20
	push r21
	push r22
	push r23
	push r24
	push r25
	push r26
	push r27
	push r28
	push r29
	push r30
	push r31
	in Temp, SREG
	push Temp
    
' Сохраняем указатель стека текущей задачи в ОЗУ.
    clr ZeroReg    
	ldi XL, &H60
	clr XH
    ld TaskCnt, X+
	mov Temp, TaskCnt
	clc
	rol Temp
	add XL, Temp
	adc XH, ZeroReg
	in Temp, SPH
	st X+, Temp
	in Temp, SPL
	st X, Temp
    
' Увеличиваем счётчик задач на единицу и сохраняем его в ОЗУ.
	inc TaskCnt
	andi TaskCnt, COUNT_MASK
    ldi XL, &H60
	clr XH
    st X+, TaskCnt
    
' Переключаемся на стек следующей задачи.    	
	mov Temp, TaskCnt
	clc
	rol Temp
	add XL, Temp
	adc XH, ZeroReg
	ld Temp, X+
	out SPH, Temp
	ld Temp, X
	out SPL, Temp

' Начальное значение счётчика нулевого таймера.
	out TCNT0, ZeroReg

' Восстанавливаем статус-регистр.
	pop Temp
	out SREG, Temp
    
' Восстанавливаем все регистры.
	pop r31
	pop r30
	pop r29
	pop r28
	pop r27
	pop r26
	pop r25
	pop r24
	pop r23
	pop r22
	pop r21
	pop r20
	pop r19
	pop r18
	pop r17
	pop r16
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop r7
    pop r6
    pop r5
    pop r4
    pop r3
    pop r2
    pop r1
    pop r0
    
$end asm

' Переход на место прерывания следующей задачи.
Return ' TIM0_OVF


Prompt:
    Data "[ATmega16]$ "

CommandShellInfo:
    Data "Command shell{013}{010}"
