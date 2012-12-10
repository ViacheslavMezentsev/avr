#pragma once


#include "FAT.h"

class CKeys {
   
public:	

    inline FCHAR_PTR operator []( uint8_t Key ) {
        
        switch ( Key ) {

            case 0: return SPSTR( "�����" );
            
            default: return 0;

        }        

    }

};


class CPanel {

public:

    // ������� ������.
    uint8_t Left;
    uint8_t Top;
    uint8_t Width;
    uint8_t Height;
    
    // ������� ������� � �� ����� �����.
    uint8_t ItemIndex;
    uint16_t ItemsCount;

    // ������� ����.
    char Path[ 128 ];

    // ���������� � ��������� ��������.
    FILINFO FileInfo;

};


class CFileManager {

private:  
    static void DrawMainMenu();
    static void DrawPanel( CPanel & Panel );
    static void WriteDateTime( CPanel & Panel, FILINFO & fno );
    static void HightlightPanel( CPanel & Panel );
    static void DrawCommandLine( CPanel & Panel );
    static void DrawFunctionKeys( CKeys & Keys );

    inline static void FormActivate();
    inline static void FormKeyDown( uint16_t Key );
    inline static void Form10msTimer();
    inline static void Form100msTimer();
    inline static void Form500msTimer();
    inline static void Form1secTimer();
    inline static void Form5secTimer();    

public:

    static CPanel * pCurrentPanel;
    
    static void DrawFrame( uint8_t Left, uint8_t Top, uint8_t Width, uint8_t Height, 
        EnColor Color, EnColor bgColor );

    static void Initialization();
    static LRESULT WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

};
