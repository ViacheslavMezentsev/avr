///////////////////////////////////////////////////////////
//  Version.h
//  Implementation of the Class CVersion
//  Created on:      5 декабря 2012 г. 13:26:51
//  Original author: Мезенцев В. Н.
///////////////////////////////////////////////////////////

#if !defined(EA_E62E5FD4_3D22_4581_B2F7_BE7DF95D3FBC__INCLUDED_)
#define EA_E62E5FD4_3D22_4581_B2F7_BE7DF95D3FBC__INCLUDED_

#include "Defines.h"

/**
 * Образ для работы с версией программы
 *
 * Внимание. Этот файл формируется автоматически. Любые внесённые
 * здесь изменения пропадут перед компиляцией.
 */
class CVersion {

public:

    inline static uint8_t GetMajor() { return 0; }
    inline static uint8_t GetMinor() { return 0; }
    inline static uint16_t GetRevision() { return 0; }
    inline static uint16_t GetBuild() { return 127; }

    inline static FCHAR_PTR GetBuildDateString() {
        return SPSTR( "5 декабря 2012 г." );
    }

};

#endif // !defined(EA_E62E5FD4_3D22_4581_B2F7_BE7DF95D3FBC__INCLUDED_)
