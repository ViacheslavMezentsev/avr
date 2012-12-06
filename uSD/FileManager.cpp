#include "Defines.h"
#include "Configuration.h"
#include "Console.h"
#include "FAT.h"
#include "FileManager.h"


// -=[ ������� ������ ]=-

extern FRESULT res;
extern FRESULT res;
extern UFDATE FDate;
extern UFTIME FTime;
extern FILINFO fno;
extern DIR dir;
extern FATFS fs;

extern char buffer[ 16 ];
extern char Version[ 16 ];
extern char * utoa_fast_div( uint32_t value, char * buffer );


// -=[ ���������� �� ����-������ ]=-


// -=[ ���������� � ��� ]=-

CKeys Keys;
CPanel LeftPanel;
CPanel RightPanel;

CPanel * CFileManager::pCurrentPanel = & LeftPanel;


/***********************
*  � � � � � � � � � �
*  ~~~~~~~~~~~~~~~~~~~
************************/


/**
 * ��������� ���� � ������
 */
void CFileManager::DrawFrame( uint8_t Left, uint8_t Top, uint8_t Width, uint8_t Height, 
        EnColor Color, EnColor bgColor ) {

    CConsole::SetTextAttributes( atOff );    
    CConsole::SetForegroundColor( Color );
    CConsole::SetBackgroundColor( bgColor );

    CConsole::MoveTo( Left, Top );

    CConsole::PutChar( ACS_DBL_ULCORNER );

    for ( uint8_t i = 0; i < Width; i++ ) CConsole::PutChar( ACS_DBL_HLINE );

    CConsole::PutChar( ACS_DBL_URCORNER );
    
    for ( uint8_t i = 0; i < Height; i++ ) {

        CConsole::MoveTo( Left, Top + i + 1 );
        CConsole::PutChar( ACS_DBL_VLINE );

        for ( uint8_t i = 0; i < Width; i++ ) CConsole::PutChar( ' ' );

        CConsole::PutChar( ACS_DBL_VLINE );

    }

    CConsole::MoveTo( Left, Top + Height + 1 );

    CConsole::PutChar( ACS_DBL_LLCORNER );

    for ( uint8_t i = 0; i < Width; i++ ) CConsole::PutChar( ACS_DBL_HLINE );

    CConsole::PutChar( ACS_DBL_LRCORNER );
    
}


void CFileManager::Initialization() {

    FLASHSTR_DECLARE( char, initpath, "/" );

    CConsole::CursorOff();

    CConsole::SetTextAttributes( atOff );
    CConsole::SetBackgroundColor( clBlack );
    CConsole::SetForegroundColor( clLightGray );
    CConsole::ClearScreen();

    CConsole::MoveTo( 1, 1 );

    LeftPanel.Left = 1;
    LeftPanel.Top = 2;
    LeftPanel.Width = 38;
    LeftPanel.Height = 20;
    LeftPanel.Line = 1;

    RightPanel.Left = 41;
    RightPanel.Top = 2;
    RightPanel.Width = 38;
    RightPanel.Height = 20;
    RightPanel.Line = 0;

    for ( uint8_t i = 0; i < 127; i++ ) {
        
        if ( initpath[i] == 0 ) {
            
            LeftPanel.Path[i] = 0;
            RightPanel.Path[i] = 0;
            break;
        }

        LeftPanel.Path[i] = initpath[i];
        RightPanel.Path[i] = initpath[i];
    }

    DrawMainMenu();
    
    DrawPanel( LeftPanel );
    DrawPanel( RightPanel );            
    
    DrawFunctionKeys( Keys );

}


/**
 * ��������� ��������� ����
 */
void CFileManager::DrawMainMenu() {

    CConsole::SetTextAttributes( atOff );        
    CConsole::MoveTo( 1, 1 );

    CConsole::SetForegroundColor( clBlack );
    CConsole::SetBackgroundColor( clWhite );
    CConsole::WriteString( SPSTR( "�������� ��������, ATmega16 @ 16 ���, ������ ������: " ), CConsole::cp1251 );
    CConsole::WriteString( Version );

#ifdef __GNUC__
    CConsole::WriteString( SPSTR( " (gnu)" ) );
#elif defined __ICCAVR__
    CConsole::WriteString( SPSTR( " (iar)" ) );
#endif

    CConsole::ClearEndOfLine();

}


void CFileManager::DrawPanel( CPanel & Panel ) {
       
    uint8_t len;

    DrawFrame( Panel.Left, Panel.Top, Panel.Width, Panel.Height, clLightGray, clBlue );

    len = Panel.Width - strlen( utoa_fast_div( fno.fsize, buffer ) );

    len /= 2;

    CConsole::MoveTo( Panel.Left + len, 2 );
    
    CConsole::PutChar( ' ' );
    
    if ( & Panel == pCurrentPanel ) {        
    
        CConsole::SetTextAttributes( atOff );
        CConsole::SetForegroundColor( clBlack );
        CConsole::SetBackgroundColor( clWhite );

        CConsole::WriteString( pCurrentPanel->Path, CConsole::cp1251 );

        CConsole::SetForegroundColor( clLightGray );
        CConsole::SetBackgroundColor( clBlue );

    } else {

        CConsole::WriteString( pCurrentPanel->Path, CConsole::cp1251 );
    }

    CConsole::PutChar( ' ' );

    CConsole::SetForegroundColor( clLightYellow );

    // ��������� �������.
    CConsole::MoveTo( Panel.Left + 5, 3 );

    CConsole::WriteString( SPSTR( "���" ), CConsole::cp1251 );

    CConsole::Move( mdForward, 8 );

    CConsole::WriteString( SPSTR( "������" ), CConsole::cp1251 );

    CConsole::Move( mdForward, 5 );

    CConsole::WriteString( SPSTR( "����   �����" ), CConsole::cp1251 );

    CConsole::SetForegroundColor( clLightGray );    

    // ��������� ��������� ��������.
    CConsole::MoveTo( Panel.Left + 13, 2 );

    CConsole::PutChar( 0xD1 );

    CConsole::Move( mdForward, 10 );

    CConsole::PutChar( 0xD1 );

    CConsole::Move( mdForward, 8 );

    CConsole::PutChar( 0xD1 );

    CConsole::Move( mdDown, 21 );
    CConsole::Move( mdBackward, 1 );

    CConsole::PutChar( 0xCF );

    CConsole::Move( mdBackward, 10 );

    CConsole::PutChar( 0xCF );

    CConsole::Move( mdBackward, 12 );

    CConsole::PutChar( 0xCF );

    for ( uint8_t i = Panel.Top + 1; i < Panel.Top + Panel.Height + 1; i++ ) {

        CConsole::MoveTo( Panel.Left + 13, i );

        if ( & Panel == pCurrentPanel ) {
        
            if ( Panel.Line == ( i - Panel.Top - 2 ) ) {        
        
                CConsole::SetTextAttributes( atOff );
                CConsole::SetForegroundColor( clBlack );
                CConsole::SetBackgroundColor( clWhite );

            } else {

                CConsole::SetForegroundColor( clLightGray );
                CConsole::SetBackgroundColor( clBlue );
            }

        }

        CConsole::PutChar( ACS_VLINE );

        CConsole::Move( mdForward, 10 );

        CConsole::PutChar( ACS_VLINE );

        CConsole::Move( mdForward, 8 );

        CConsole::PutChar( ACS_VLINE );

    }

    // ������������
    res = CFAT::Mount( & fs );

    // ��������� ���������� (�����)
    res = CFAT::OpenDir( & dir, Panel.Path );

    if ( res == FR_OK ) {

        for ( uint8_t i = Panel.Top + 2; i < Panel.Top + Panel.Height + 1; i++ ) {

            CConsole::MoveTo( Panel.Left + 1, i );

            res = CFAT::ReadDir( & dir, & fno );

            if ( res != FR_OK || fno.fname[0] == 0 ) break;

            if ( & Panel == pCurrentPanel ) {
            
                if ( Panel.Line == ( i - Panel.Top - 2 ) ) {        
            
                    CConsole::SetTextAttributes( atOff );
                    CConsole::SetForegroundColor( clBlack );
                    CConsole::SetBackgroundColor( clWhite );

                } else {

                    CConsole::SetForegroundColor( clLightGray );
                    CConsole::SetBackgroundColor( clBlue );
                }

            }

            // ���� ������ - �����
            if ( fno.fattrib & AM_DIR ) {

                if ( !( ( & Panel == pCurrentPanel ) 
                    && ( Panel.Line == ( i - Panel.Top - 2 ) ) ) ) {
                    
                    CConsole::SetForegroundColor( clLightGreen );
                }

                // ���
                CConsole::WriteString( fno.fname, CConsole::cp1251 );

                len = 11 - strlen( fno.fname );

                do CConsole::PutChar( ' ' ); while ( len-- );

                CConsole::MoveTo( Panel.Left + 14, i );

                CConsole::WriteString( SPSTR( "  [�����] " ), CConsole::cp1251 );

            // ���� ������ - ����
            } else {

                if ( !( ( & Panel == pCurrentPanel ) 
                    && ( Panel.Line == ( i - Panel.Top - 2 ) ) ) ) {
                    
                    CConsole::SetForegroundColor( clLightGray );
                }

                // ���
                CConsole::WriteString( fno.fname, CConsole::cp1251 );

                CConsole::MoveTo( Panel.Left + 14, i );

                len = 9 - strlen( utoa_fast_div( fno.fsize, buffer ) );

                do CConsole::PutChar( ' ' ); while ( len-- );

                CConsole::WriteString( utoa_fast_div( fno.fsize, buffer ) );

            }

            CConsole::MoveTo( Panel.Left + 25, i );

            // ����� ����
            FDate.Value = fno.fdate;

            // ����
            CConsole::PutChar( FDate.fdate.Day / 10 + '0' );
            CConsole::PutChar( FDate.fdate.Day % 10 + '0' );
            CConsole::PutChar( '.' );

            // �����
            CConsole::PutChar( FDate.fdate.Month / 10 + '0' );
            CConsole::PutChar( FDate.fdate.Month % 10 + '0' );
            CConsole::PutChar( '.' );

            // ���
            uint16_t tmp = 1980U + FDate.fdate.Year;

            tmp %= 1000U;
            tmp %= 100;

            CConsole::PutChar( ( uint8_t ) ( tmp / 10 ) + '0' );
            CConsole::PutChar( ( uint8_t ) ( tmp % 10 ) + '0' );

            CConsole::Move( mdForward, 1 );

            // ����� �������
            FTime.Value = fno.ftime;

            // ����
            CConsole::PutChar( FTime.ftime.Hour / 10 + '0' );
            CConsole::PutChar( FTime.ftime.Hour % 10 + '0' );

            CConsole::PutChar( ':' );

            // ������
            CConsole::PutChar( FTime.ftime.Minute / 10 + '0' );
            CConsole::PutChar( FTime.ftime.Minute % 10 + '0' );

        } // for

    } // if

    // �����������
    res = CFAT::Mount( NULL );

}


void CFileManager::DrawCommandLine( CPanel & Panel ) {

    // ������� �����������
    CConsole::SetTextAttributes( atOff );    
    CConsole::SetForegroundColor( clLightGreen );

    CConsole::MoveTo( 1, 24 );
    
    CConsole::PutChar( '[' );
    CConsole::WriteString( Panel.Path );
    CConsole::WriteString( SPSTR( "]$ " ) );

    CConsole::CursorOn();

}


/**
 * ���������� ������� �������������� ������.
 */
void CFileManager::DrawFunctionKeys( CKeys & Keys ) {

    char buf[3];

    CConsole::SetTextAttributes( atOff );
    CConsole::MoveTo( 1, 25 );

    // ���������� �������������� �������.
    for ( uint8_t i = 0; i < 10; i++ ) {

        if ( Keys[i] != 0 ) {
            
            CConsole::SetForegroundColor( clRed );
            CConsole::SetBackgroundColor( clWhite );
            
            // �����������
            CConsole::PutChar( ' ' );
            
            CConsole::WriteString( utoa_fast_div( i, buf ) );
            
            // �����������
            CConsole::PutChar( ' ' );

            CConsole::SetForegroundColor( clBlack );
            CConsole::WriteString( Keys[i], CConsole::cp1251 );

        }

    }

    // ��������� ������ ������������ �� ����� ������.
    CConsole::ClearEndOfLine();

}


void CFileManager::Run() {

    char * cmd;

    // ������� ����������� ��������� ������.
    DrawCommandLine( * pCurrentPanel );

    CConsole::SetForegroundColor( clLightGray );

    do {
    
        cmd = CConsole::ReadString( buffer );

    } while ( ! ( ( cmd[0] == 'q' ) && ( cmd[1] == 0 ) ) );

}