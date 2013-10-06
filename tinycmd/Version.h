///////////////////////////////////////////////////////////
//  Version.h
//  Implementation of the Class CVersion
//  Created on:      4 ������� 2013 �. 12:27:45
//  Original author: �������� �. �.
///////////////////////////////////////////////////////////

#if !defined(EA_E62E5FD4_3D22_4581_B2F7_BE7DF95D3FBC__INCLUDED_)
#define EA_E62E5FD4_3D22_4581_B2F7_BE7DF95D3FBC__INCLUDED_

#include "Defines.h"

/**
 * ����� ��� ������ � ������� ���������.
 *
 * ��������. ���� ���� ����������� �������������. ����� ��������
 * ����� ��������� �������� ����� �����������.
 */
class CVersion {

public:

    inline static uint8_t GetMajor() { return 0; }
    inline static uint8_t GetMinor() { return 0; }
    inline static uint16_t GetRevision() { return 0; }
    inline static uint16_t GetBuild() { return 26; }

    inline static FCHAR_PTR GetVersionString() {
        return SPSTR( "0.0.0.26" );
    }
    inline static FCHAR_PTR GetBuildDateString() {
        return SPSTR( "4 ������� 2013 �." );
    }

};

#endif // !defined(EA_E62E5FD4_3D22_4581_B2F7_BE7DF95D3FBC__INCLUDED_)
