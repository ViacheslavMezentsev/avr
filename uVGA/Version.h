///////////////////////////////////////////////////////////
//  Version.h
//  Implementation of the Class CVersion
//  Created on:      23 ноября 2012 г. 8:46:45
//  Original author: Мезенцев В. Н.
///////////////////////////////////////////////////////////

#if !defined(EA_E62E5FD4_3D22_4581_B2F7_BE7DF95D3FBC__INCLUDED_)
#define EA_E62E5FD4_3D22_4581_B2F7_BE7DF95D3FBC__INCLUDED_

#include "Defines.h"

/**
 * Образ для работы с версией программы
 */
class CVersion {

public:

    inline static uint8_t GetMajor() { return 0; }
    inline static uint8_t GetMinor() { return 0; }
    inline static uint16_t GetRevision() { return 0; }
    inline static uint16_t GetBuild() { return 8; }

    inline static FCHAR_PTR GetBuildDateString() {
        FLASHSTR_DECLARE( char, szBuildDateString, "23 ноября 2012 г." );
        return szBuildDateString;
    }

};

#endif // !defined(EA_E62E5FD4_3D22_4581_B2F7_BE7DF95D3FBC__INCLUDED_)
