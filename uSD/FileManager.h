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


class CPanel {

public:

    uint8_t Left;
    uint8_t Top;
    uint8_t Width;
    uint8_t Height;
    
    uint8_t Line;

    char Path[ 128 ];

};


class CFileManager {

private:
    
    static void DrawFrame( uint8_t Left, uint8_t Top, uint8_t Width, uint8_t Height, 
        EnColor Color, EnColor bgColor );

    static void DrawMainMenu();
    static void DrawPanel( CPanel & Panel );
    static void DrawCommandLine( CPanel & Panel );
    static void DrawFunctionKeys( CKeys & Keys );

public:

    static CPanel * pCurrentPanel;

    static void Initialization();
    static void Run();

};
