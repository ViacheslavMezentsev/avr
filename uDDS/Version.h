///////////////////////////////////////////////////////////
//  Version.h
//  Implementation of the Class CVersion
//  Created on:      26 ������� 2013 �. 19:51:56
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
    inline static uint32_t GetBuild() { return 5047; }
    inline static uint32_t GetRevision() { return 35758; }

    inline static FCHAR_PTR GetVersionString() {
        return SPSTR( "0.0.5047.35758" );
    }
    inline static FCHAR_PTR GetBuildDateString() {
        return SPSTR( "26 ������� 2013 �." );
    }

};

#endif // !defined(EA_E62E5FD4_3D22_4581_B2F7_BE7DF95D3FBC__INCLUDED_)
