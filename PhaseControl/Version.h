///////////////////////////////////////////////////////////
//  Version.h
//  Implementation of the Class CVersion
//  Created on:      17 апреля 2013 г. 4:43:20
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
    inline static uint16_t GetBuild() { return 163; }

    inline static FCHAR_PTR GetBuildDateString() {
        return SPSTR( "17 апреля 2013 г." );
    }

};

#endif // !defined(EA_E62E5FD4_3D22_4581_B2F7_BE7DF95D3FBC__INCLUDED_)
