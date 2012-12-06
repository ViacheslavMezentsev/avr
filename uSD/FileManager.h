#pragma once


class CKeys {
   
public:	

    inline FCHAR_PTR operator []( uint8_t Key ) {
        
        switch ( Key ) {

            case 0: return SPSTR( "Выход" );
            
            default: return 0;

        }        

    }

};


class CFileManager {

private:
    
    inline static void DrawFrame( uint8_t Left, uint8_t Top, uint8_t Width, uint8_t Height, 
        EnColor Color, EnColor bgColor );

public:

    static void Initialization();
    static void DrawMainMenu();
    static void DrawLeftPanel();
    static void DrawRightPanel();
    static void DrawCommandLine();
    static void DrawFunctionKeys( CKeys & Keys );
    static void Run();

};
