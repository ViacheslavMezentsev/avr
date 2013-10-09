///////////////////////////////////////////////////////////
//  Version.h
//  Implementation of the Class CVersion
//  Created on:      8 ������� 2013 �. 4:20:31
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

    inline static uint32_t GetMajor() { return 0; }
    inline static uint32_t GetMinor() { return 0; }
    inline static uint32_t GetBuild() { return 5029; }
    inline static uint32_t GetRevision() { return 7815; }

    inline static FCHAR_PTR GetVersionString() {
        return SPSTR( "0.0.5029.7815" );
    }
    inline static FCHAR_PTR GetBuildDateString() {
        return SPSTR( "8 ������� 2013 �." );
    }

};

#endif // !defined(EA_E62E5FD4_3D22_4581_B2F7_BE7DF95D3FBC__INCLUDED_)
