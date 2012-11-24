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
#include "MCU.h"
#include "MainUnit.h"


// ����� ����� � �������� ���������
int main() {

    // ������������� ����������������
    CMCU::Initialization();

    // ������ ��������� ������
    HRESULT hResult = CMCU::MainThreadProcedure();

    return 0;

}
