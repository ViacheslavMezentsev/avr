///////////////////////////////////////////////////////////
//  Version.h
//  Implementation of the Class CVersion
//  Created on:      28 октября 2015 г. 21:24:41
//  Original author: Мезенцев В. Н.
///////////////////////////////////////////////////////////

#if !defined(EA_E62E5FD4_3D22_4581_B2F7_BE7DF95D3FBC__INCLUDED_)
#define EA_E62E5FD4_3D22_4581_B2F7_BE7DF95D3FBC__INCLUDED_

#include "Defines.h"

/**
 * Образ для работы с версией программы.
 *
 * Внимание. Этот файл формируется автоматически. Любые внесённые
 * здесь изменения пропадут перед компиляцией.
 */
class CVersion {

public:

    inline static uint32_t GetMajor() { return 0; }
    inline static uint32_t GetMinor() { return 0; }
    inline static uint32_t GetBuild() { return 0; }
    inline static uint32_t GetRevision() { return 31; }

    inline static FCHAR_PTR GetVersionString() {
        return SPSTR( "0.0.0.31" );
    }
    inline static FCHAR_PTR GetBuildDateString() {
        return SPSTR( "28 октября 2015 г." );
    }

};

#endif // !defined(EA_E62E5FD4_3D22_4581_B2F7_BE7DF95D3FBC__INCLUDED_)
