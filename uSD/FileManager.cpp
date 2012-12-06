#include "Defines.h"
#include "Configuration.h"
#include "Console.h"
#include "FAT.h"
#include "FileManager.h"


// -=[ Внешние ссылки ]=-

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


// -=[ Постоянные во флеш-памяти ]=-


// -=[ Переменные в ОЗУ ]=-

CKeys Keys;
CPanel LeftPanel;
CPanel RightPanel;

CPanel * CFileManager::pCurrentPanel = & LeftPanel;


/***********************
*  Р Е А Л И З А Ц И Я
*  ~~~~~~~~~~~~~~~~~~~
************************/


/**
 * Отрисовка окна с рамкой
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
 * Отрисовка заголовка окна
 */
void CFileManager::DrawMainMenu() {

    CConsole::SetTextAttributes( atOff );        
    CConsole::MoveTo( 1, 1 );

    CConsole::SetForegroundColor( clBlack );
    CConsole::SetBackgroundColor( clWhite );
    CConsole::WriteString( SPSTR( "Файловый менеджер, ATmega16 @ 16 МГц, версия сборки: " ), CConsole::cp1251 );
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

    // Заголовки колонок.
    CConsole::MoveTo( Panel.Left + 5, 3 );

    CConsole::WriteString( SPSTR( "Имя" ), CConsole::cp1251 );

    CConsole::Move( mdForward, 8 );

    CConsole::WriteString( SPSTR( "Размер" ), CConsole::cp1251 );

    CConsole::Move( mdForward, 5 );

    CConsole::WriteString( SPSTR( "Дата   Время" ), CConsole::cp1251 );

    CConsole::SetForegroundColor( clLightGray );    

    // Добавляем стыкующие элементы.
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

    // Монтирование
    res = CFAT::Mount( & fs );

    // Открываем директорию (папку)
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

            // Если объект - папка
            if ( fno.fattrib & AM_DIR ) {

                if ( !( ( & Panel == pCurrentPanel ) 
                    && ( Panel.Line == ( i - Panel.Top - 2 ) ) ) ) {
                    
                    CConsole::SetForegroundColor( clLightGreen );
                }

                // Имя
                CConsole::WriteString( fno.fname, CConsole::cp1251 );

                len = 11 - strlen( fno.fname );

                do CConsole::PutChar( ' ' ); while ( len-- );

                CConsole::MoveTo( Panel.Left + 14, i );

                CConsole::WriteString( SPSTR( "  [Папка] " ), CConsole::cp1251 );

            // Если объект - файл
            } else {

                if ( !( ( & Panel == pCurrentPanel ) 
                    && ( Panel.Line == ( i - Panel.Top - 2 ) ) ) ) {
                    
                    CConsole::SetForegroundColor( clLightGray );
                }

                // Имя
                CConsole::WriteString( fno.fname, CConsole::cp1251 );

                CConsole::MoveTo( Panel.Left + 14, i );

                len = 9 - strlen( utoa_fast_div( fno.fsize, buffer ) );

                do CConsole::PutChar( ' ' ); while ( len-- );

                CConsole::WriteString( utoa_fast_div( fno.fsize, buffer ) );

            }

            CConsole::MoveTo( Panel.Left + 25, i );

            // Вывод даты
            FDate.Value = fno.fdate;

            // День
            CConsole::PutChar( FDate.fdate.Day / 10 + '0' );
            CConsole::PutChar( FDate.fdate.Day % 10 + '0' );
            CConsole::PutChar( '.' );

            // Месяц
            CConsole::PutChar( FDate.fdate.Month / 10 + '0' );
            CConsole::PutChar( FDate.fdate.Month % 10 + '0' );
            CConsole::PutChar( '.' );

            // Год
            uint16_t tmp = 1980U + FDate.fdate.Year;

            tmp %= 1000U;
            tmp %= 100;

            CConsole::PutChar( ( uint8_t ) ( tmp / 10 ) + '0' );
            CConsole::PutChar( ( uint8_t ) ( tmp % 10 ) + '0' );

            CConsole::Move( mdForward, 1 );

            // Вывод времени
            FTime.Value = fno.ftime;

            // Часы
            CConsole::PutChar( FTime.ftime.Hour / 10 + '0' );
            CConsole::PutChar( FTime.ftime.Hour % 10 + '0' );

            CConsole::PutChar( ':' );

            // Минуты
            CConsole::PutChar( FTime.ftime.Minute / 10 + '0' );
            CConsole::PutChar( FTime.ftime.Minute % 10 + '0' );

        } // for

    } // if

    // Отмонтируем
    res = CFAT::Mount( NULL );

}


void CFileManager::DrawCommandLine( CPanel & Panel ) {

    // Выводим приглашение
    CConsole::SetTextAttributes( atOff );    
    CConsole::SetForegroundColor( clLightGreen );

    CConsole::MoveTo( 1, 24 );
    
    CConsole::PutChar( '[' );
    CConsole::WriteString( Panel.Path );
    CConsole::WriteString( SPSTR( "]$ " ) );

    CConsole::CursorOn();

}


/**
 * Отображаем линейку функциональных клавиш.
 */
void CFileManager::DrawFunctionKeys( CKeys & Keys ) {

    char buf[3];

    CConsole::SetTextAttributes( atOff );
    CConsole::MoveTo( 1, 25 );

    // Перебираем функциональные клавиши.
    for ( uint8_t i = 0; i < 10; i++ ) {

        if ( Keys[i] != 0 ) {
            
            CConsole::SetForegroundColor( clRed );
            CConsole::SetBackgroundColor( clWhite );
            
            // Разделитель
            CConsole::PutChar( ' ' );
            
            CConsole::WriteString( utoa_fast_div( i, buf ) );
            
            // Разделитель
            CConsole::PutChar( ' ' );

            CConsole::SetForegroundColor( clBlack );
            CConsole::WriteString( Keys[i], CConsole::cp1251 );

        }

    }

    // Заполняем пустое пространство до конца строки.
    CConsole::ClearEndOfLine();

}


void CFileManager::Run() {

    char * cmd;

    // Выводим приглашение командной строки.
    DrawCommandLine( * pCurrentPanel );

    CConsole::SetForegroundColor( clLightGray );

    do {
    
        cmd = CConsole::ReadString( buffer );

    } while ( ! ( ( cmd[0] == 'q' ) && ( cmd[1] == 0 ) ) );

}