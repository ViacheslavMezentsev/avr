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
#include "EFS.h"
#include "MCU.h"
#include "DDS.h"
#include "CommandShell.h"


// ����� ����� � �������� ���������.
int main() {

    // ������������� �������� EFS.
    CEFS::Initialization();

    // ������������� ����������������.
    CMCU::Initialization();

    // ������������� ������ DDS.
    CDDS::Initialization();

    // ������������� ���������� ��������������.
    CCommandShell::Initialization();

    CCommandShell::FormActivate();

    // ������ ��������� ������.
    HRESULT hResult = CMCU::MainThreadProcedure();

    return 0;

}
