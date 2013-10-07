///////////////////////////////////////////////////////////
//  MemoryViewer.h
//  Implementation of the Class CMemoryViewer
//  Created on:      06-окт-2013 17:44:50
//  Original author: Слава
///////////////////////////////////////////////////////////

#if !defined(EA_705342B6_C1DE_4db2_AC1A_1F0D69ACDF02__INCLUDED_)
#define EA_705342B6_C1DE_4db2_AC1A_1F0D69ACDF02__INCLUDED_

class CKeys {

public:	

    inline uint8_t Count() { return 4; }

    inline FCHAR_PTR operator []( uint8_t Key ) {

        switch ( Key ) {

            case 0: return SPSTR( " Esc |Выход" );
            case 1: return SPSTR( " 0 |Очистить" );
            case 2: return SPSTR( " 1 |Создать" );
            case 3: return SPSTR( " 2 |Удалить" );

            default: return 0;

        }        

    }

};

class CMemoryViewer {

private:

    static void DrawTitle();
    static void DrawMemory();
    static void DrawFunctionKeys( CKeys & Keys );

public:

    static void Initialization();

    static void FormActivate();
    static void FormKeyDown( uint16_t Key );

};

#endif // !defined(EA_705342B6_C1DE_4db2_AC1A_1F0D69ACDF02__INCLUDED_)
