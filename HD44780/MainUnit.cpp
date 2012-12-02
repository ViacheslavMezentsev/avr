/*
* ������: ������ ��� ATmega16
* ����������: ATmega16 @ 16 ���
*
* ��� ������� �� ������ ���������� ��������� ������������ �����
* � ISIS, ������� ��� ��� ����������� ����������� ����:
* - gccout.elf, ���� �������� ������ � VS2008/VS2010 (avrgcc);
* - iccout_ubrof8.d90, ���� �������� ������ � IAR (iccavr).
*
*  ����� ��������� � ����� ������� (����� ����������).
*
* ��������. ����� ����������� ����������� ��������� ����� �
* makefile:
* - ����� ������ ���������� (CPPSRC =);
* - External Memory Options ��� gcc (������� ���);
* - ������� ����������� (0, 1, 2, 3, s).
*
* TODO:
*
*/

#include "Defines.h"
#include "Configuration.h"
#include "LCD.h"
#include "MCU.h"


// ����� ����� � �������� ���������
int main() {

    // ������������� ����������������
    CMCU::Initialization();

    // ��������� ������������� �������� ��� ����������
    CLCD::Initialization( CLCD::LCD_DRIVER_HD44780 );

    // ������ ��������� ������
    HRESULT hResult = CMCU::MainThreadProcedure();

    return 0;

}
