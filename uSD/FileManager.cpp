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

    CConsole::PutChar( ACS_DBL_HLINE );

    for ( uint8_t i = 0; i < Width / 2 - 3; i++ ) CConsole::PutChar( ACS_DBL_HLINE );

    CConsole::WriteString( SPSTR( " A:\\ " ), CConsole::cp1251 );

    for ( uint8_t i = 0; i < Width / 2 - 1; i++ ) CConsole::PutChar( ACS_DBL_HLINE );

    CConsole::PutChar( ACS_DBL_URCORNER );

    for ( uint8_t i = 0; i < Height; i++ ) {

        CConsole::MoveTo( Left, Top + i + 1 );
        CConsole::PutChar( ACS_DBL_VLINE );

        CConsole::PutChar( ' ' );

        for ( uint8_t j = 0; j < Width; j++ ) CConsole::PutChar( ' ' );

        CConsole::PutChar( ' ' );

        CConsole::PutChar( ACS_DBL_VLINE );

    }

    CConsole::MoveTo( Left, Top + Height + 1 );

    CConsole::PutChar( ACS_DBL_LLCORNER );

    for ( uint8_t i = 0; i < Width + 2; i++ ) CConsole::PutChar( ACS_DBL_HLINE );

    CConsole::PutChar( ACS_DBL_LRCORNER );
    
}


void CFileManager::Initialization() {

    CConsole::CursorOff();

    CConsole::SetTextAttributes( atOff );
    CConsole::SetBackgroundColor( clBlack );
    CConsole::SetForegroundColor( clLightGray );
    CConsole::ClearScreen();

    CConsole::MoveTo( 1, 1 );

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


void CFileManager::DrawLeftPanel() {
       
    uint8_t len;

    DrawFrame( 1, 2, 36, 20, clLightGray, clBlue );

    CConsole::SetForegroundColor( clLightYellow );

    // Заголовки колонок.
    CConsole::MoveTo( 6, 3 );

    CConsole::WriteString( SPSTR( "Имя" ), CConsole::cp1251 );

    CConsole::Move( mdForward, 8 );

    CConsole::WriteString( SPSTR( "Размер" ), CConsole::cp1251 );

    CConsole::Move( mdForward, 5 );

    CConsole::WriteString( SPSTR( "Дата   Время" ), CConsole::cp1251 );

    CConsole::SetForegroundColor( clLightGray );    

    // Добавляем стыкующие элементы.
    CConsole::MoveTo( 14, 2 );

    CConsole::PutChar( 0xD1 );

    CConsole::Move( mdForward, 10 );

    CConsole::PutChar( 0xD1 );

    CConsole::Move( mdDown, 21 );
    CConsole::Move( mdBackward, 1 );

    CConsole::PutChar( 0xCF );

    CConsole::Move( mdBackward, 12 );

    CConsole::PutChar( 0xCF );

    for ( uint8_t i = 3; i < 23; i++ ) {

        CConsole::MoveTo( 14, i );

        CConsole::PutChar( ACS_VLINE );

        CConsole::Move( mdForward, 10 );

        CConsole::PutChar( ACS_VLINE );

    }

    // Монтирование
    res = CFAT::Mount( & fs );

    // Открываем директорию (папку)
    res = CFAT::OpenDir( & dir, SPSTR( "/" ) );

    if ( res == FR_OK ) {

        for ( uint8_t i = 4; i < 23; i++ ) {

            CConsole::MoveTo( 2, i );

            res = CFAT::ReadDir( & dir, & fno );

            if ( res != FR_OK || fno.fname[0] == 0 ) break;

            // Если объект - папка
            if ( fno.fattrib & AM_DIR ) {

                CConsole::SetForegroundColor( clLightGreen );

                // Имя
                CConsole::WriteString( fno.fname, CConsole::cp1251 );

                CConsole::MoveTo( 15, i );

                CConsole::WriteString( SPSTR( "  [Папка] " ), CConsole::cp1251 );

            // Если объект - файл
            } else {

                CConsole::SetForegroundColor( clLightGray );

                // Имя
                CConsole::WriteString( fno.fname, CConsole::cp1251 );

                CConsole::MoveTo( 15, i );

                len = 9 - strlen( utoa_fast_div( fno.fsize, buffer ) );

                do CConsole::PutChar( ' ' ); while ( len-- );

                CConsole::WriteString( utoa_fast_div( fno.fsize, buffer ) );

            }

            CConsole::MoveTo( 26, i );

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

            CConsole::PutChar( ' ' );

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


void CFileManager::DrawRightPanel() {

    uint8_t len;

    DrawFrame( 41, 2, 36, 20, clLightGray, clBlue );

    CConsole::SetForegroundColor( clLightYellow );

    // Заголовки колонок.
    CConsole::MoveTo( 46, 3 );

    CConsole::WriteString( SPSTR( "Имя" ), CConsole::cp1251 );

    CConsole::Move( mdForward, 8 );

    CConsole::WriteString( SPSTR( "Размер" ), CConsole::cp1251 );

    CConsole::Move( mdForward, 6 );

    CConsole::WriteString( SPSTR( "Дата" ), CConsole::cp1251 );

    CConsole::SetForegroundColor( clLightGray );    

    // Добавляем стыкующие элементы.
    CConsole::MoveTo( 54, 2 );

    CConsole::PutChar( 0xD1 );

    CConsole::Move( mdForward, 10 );

    CConsole::PutChar( 0xD1 );

    CConsole::Move( mdDown, 21 );
    CConsole::Move( mdBackward, 1 );

    CConsole::PutChar( 0xCF );

    CConsole::Move( mdBackward, 12 );

    CConsole::PutChar( 0xCF );

    for ( uint8_t i = 3; i < 23; i++ ) {

        CConsole::MoveTo( 54, i );

        CConsole::PutChar( ACS_VLINE );

        CConsole::Move( mdForward, 10 );

        CConsole::PutChar( ACS_VLINE );

    }

    // Монтирование
    res = CFAT::Mount( & fs );

    // Открываем директорию (папку)
    res = CFAT::OpenDir( & dir, SPSTR( "/" ) );

    if ( res == FR_OK ) {

        for ( uint8_t i = 4; i < 23; i++ ) {

            CConsole::MoveTo( 42, i );

            res = CFAT::ReadDir( & dir, & fno );

            if ( res != FR_OK || fno.fname[0] == 0 ) break;

            // Если объект - папка
            if ( fno.fattrib & AM_DIR ) {

                CConsole::SetForegroundColor( clLightGreen );

                // Имя
                CConsole::WriteString( fno.fname, CConsole::cp1251 );

                CConsole::MoveTo( 55, i );

                CConsole::WriteString( SPSTR( "  [Папка] " ), CConsole::cp1251 );

            // Если объект - файл
            } else {

                CConsole::SetForegroundColor( clLightGray );

                // Имя
                CConsole::WriteString( fno.fname, CConsole::cp1251 );

                CConsole::MoveTo( 55, i );

                len = 9 - strlen( utoa_fast_div( fno.fsize, buffer ) );

                do CConsole::PutChar( ' ' ); while ( len-- );

                CConsole::WriteString( utoa_fast_div( fno.fsize, buffer ) );

            }

        } // for

    } // if

    // Отмонтируем
    res = CFAT::Mount( NULL );

}


void CFileManager::DrawCommandLine() {

    // Выводим приглашение
    CConsole::SetTextAttributes( atOff );    
    CConsole::SetForegroundColor( clLightGreen );

    CConsole::MoveTo( 1, 24 );
    CConsole::WriteString( SPSTR( "[A:\\]$ " ) );

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

    DrawCommandLine();

    CConsole::SetForegroundColor( clLightGray );

    do {
    
        cmd = CConsole::ReadString( buffer );

    } while ( ! ( ( cmd[0] == 'q' ) && ( cmd[1] == 0 ) ) );

}