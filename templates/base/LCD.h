#pragma once

// TODO: Переписать под свой класс

// Define type of interface used to access the LCD
//  LCD_MEMORY_INTERFACE:
//      To use this mode you must supply the necessary hardware to connect the
//      LCD to the CPU's memory bus.  The CONTROL and DATA registers of the LCD
//      (HD44780 chip) must appear in the CPU's memory map.  This mode is faster
//      than the port interface but requires a little extra hardware to make it
//      work.  It is especially useful when your CPU is already configured to
//      use an external memory bus for other purposes (like accessing memory).
//
// LCD_PORT_INTERFACE:
//      This mode allows you to connect the control and data lines of the LCD
//      directly to the I/O port pins (no interfacing hardware is needed),
//      but it generally runs slower than the LCD_MEMORY_INTERFACE.
//      Depending on your needs, when using the LCD_PORT_INTERFACE, the LCD may
//      be accessed in 8-bit or 4-bit mode.  In 8-bit mode, one whole I/O port
//      (pins 0-7) is required for the LCD data lines, but transfers are faster.
//      In 4-bit mode, only I/O port pins 4-7 are needed for data lines, but LCD
//      access is slower.  In either mode, three additional port pins are
//      required for the LCD interface control lines (RS, R/W, and E).

/*
* // Пример.
* // Разводка выводов для работы с ЖКИ
* // Задаём DDR и PORT для управления LCD_E, LCD_RS и LCD_RW
*
* // Конфигурация LCD дисплея
*
* // Число видимых строк
*#define LCD_LINES   2   
* // Длина строки (в символах)
*#define LCD_LINE_LENGTH 16  
*
* // Адреса строк в DDRAM
*#define LCD_LINE0_DDRAMADDR 0x00
*#define LCD_LINE1_DDRAMADDR 0x40
*#define LCD_LINE2_DDRAMADDR 0x00
*#define LCD_LINE3_DDRAMADDR 0x40
*
* // Разводка выводов для работы с ЖКИ
* // Задаём DDR и PORT для управления LCD_E и LCD_RS
*
* // Линия: Enable Signal
*#define LCD_DDR_NAME_En     SET_DDR_NAME( B )   // -> DDRB
*#define LCD_PORT_NAME_En    SET_PORT_NAME( B )  // -> PORTB
*#define LCD_PIN_NAME_En     SET_PIN_NAME( B )   // -> PINB
*#define LCD_PIN_En          SET_PORT_PIN_NUMBER( B, 0 ) // -> PB0
*
* // Линия: Data/Instruction Select
*#define LCD_DDR_NAME_RS     SET_DDR_NAME( C )   // -> DDRC
*#define LCD_PORT_NAME_RS    SET_PORT_NAME( C )  // -> PORTC
*#define LCD_PIN_NAME_RS     SET_PIN_NAME( C )   // -> PINC
*#define LCD_PIN_RS          SET_PORT_PIN_NUMBER( C, 6 ) // -> PC6
*
* // Линия: Read/Write Select
* //#define LCD_RW_UNUSED
*
*#ifndef LCD_RW_UNUSED
*    #define LCD_DDR_NAME_RW     SET_DDR_NAME( B )   // -> DDRB
*    #define LCD_PORT_NAME_RW    SET_PORT_NAME( B )  // -> PORTB
*    #define LCD_PIN_NAME_RW     SET_PIN_NAME( B )   // -> PINB
*    #define LCD_PIN_RW          SET_PORT_PIN_NUMBER( B, 1 ) // -> PB1
*#endif
*
* // Количество линий в шине для работы с ЖКИ
*#define LCD_DATA_LENGTH_4BIT
* //#define LCD_DATA_LENGTH_8BIT
*
*#ifdef LCD_DATA_LENGTH_4BIT
*
*    #define LCD_DDR_NAME_D4     SET_DDR_NAME( B )       // -> DDRB
*    #define LCD_PORT_NAME_D4    SET_PORT_NAME( B )      // -> PORTB
*    #define LCD_PIN_NAME_D4     SET_PIN_NAME( B )       // -> PINB
*    #define LCD_PIN_D4          SET_PORT_PIN_NUMBER( B, 4 ) // -> PB4
*
*    #define LCD_DDR_NAME_D5     SET_DDR_NAME( B )       // -> DDRB
*    #define LCD_PORT_NAME_D5    SET_PORT_NAME( B )      // -> PORTB
*    #define LCD_PIN_NAME_D5     SET_PIN_NAME( B )       // -> PINB
*    #define LCD_PIN_D5          SET_PORT_PIN_NUMBER( B, 5 ) // -> PB5
*
*    #define LCD_DDR_NAME_D6     SET_DDR_NAME( B )       // -> DDRB
*    #define LCD_PORT_NAME_D6    SET_PORT_NAME( B )      // -> PORTB
*    #define LCD_PIN_NAME_D6     SET_PIN_NAME( B )       // -> PINB
*    #define LCD_PIN_D6          SET_PORT_PIN_NUMBER( B, 6 ) // -> PB6
*
*    #define LCD_DDR_NAME_D7     SET_DDR_NAME( B )       // -> DDRB
*    #define LCD_PORT_NAME_D7    SET_PORT_NAME( B )      // -> PORTB
*    #define LCD_PIN_NAME_D7     SET_PIN_NAME( B )       // -> PINB
*    #define LCD_PIN_D7          SET_PORT_PIN_NUMBER( B, 7 ) // -> PB7
*
*#endif
*
*#ifdef LCD_DATA_LENGTH_8BIT
*    #define LCD_DDR_D0D7 SET_DDR_NAME( F )   // -> DDRF
*    #define LCD_PORT_D0D7 SET_PORT_NAME( F )   // -> PORTF
*    #define LCD_PIN_D0D7 PINF
*#endif
*
* // Задержки ожидания выполнения операций драйвером ЖКИ
*#define LCD_DELAYS_DATA_US 50
*#define LCD_DELAYS_COMMAND_US 50
*#define LCD_DELAYS_CLEAR_MS 2.0
*
*/

#ifndef LCD_DELAYS_DATA_US
    #error "LCD_DELAYS_DATA_US not defined."
#endif

#ifndef LCD_DELAYS_COMMAND_US
    #error "LCD_DELAYS_COMMAND_US not defined."
#endif

#ifndef LCD_DELAYS_CLEAR_MS
    #error "LCD_DELAYS_CLEAR_MS not defined."
#endif

#ifndef LCD_DDR_NAME_En
    #error "LCD_DDR_NAME_En not defined."
#endif

#ifndef LCD_PORT_NAME_En
    #error "LCD_PORT_NAME_En not defined."
#endif

#ifndef LCD_PIN_NAME_En
    #error "LCD_PIN_NAME_En not defined."
#endif

#ifndef LCD_PIN_En
    #error "LCD_PIN_En not defined."
#endif

#ifndef LCD_DDR_NAME_RS
    #error "LCD_DDR_NAME_RS not defined."
#endif

#ifndef LCD_PORT_NAME_RS
    #error "LCD_PORT_NAME_RS not defined."
#endif

#ifndef LCD_PIN_NAME_RS
    #error "LCD_PIN_NAME_RS not defined."
#endif

#ifndef LCD_PIN_RS
    #error "LCD_PIN_RS not defined."
#endif

#ifndef LCD_RW_UNUSED

    #ifndef LCD_DDR_NAME_RW
        #error "LCD_DDR_NAME_RW not defined."
    #endif

    #ifndef LCD_PORT_NAME_RW
        #error "LCD_PORT_NAME_RW not defined."
    #endif

    #ifndef LCD_PIN_NAME_RW
        #error "LCD_PIN_NAME_RW not defined."
    #endif

    #ifndef LCD_PIN_RW
        #error "LCD_PIN_RW not defined."
    #endif
    
#endif

#ifdef LCD_DATA_LENGTH_4BIT

    #ifndef LCD_DDR_NAME_D4
        #error "LCD_DDR_NAME_D4 not defined."
    #endif

    #ifndef LCD_PORT_NAME_D4
        #error "LCD_PORT_NAME_D4 not defined."
    #endif

    #ifndef LCD_PIN_NAME_D4
        #error "LCD_PIN_NAME_D4 not defined."
    #endif

    #ifndef LCD_PIN_D4
        #error "LCD_PIN_D4 not defined."
    #endif
    
    #ifndef LCD_DDR_NAME_D5
        #error "LCD_DDR_NAME_D5 not defined."
    #endif

    #ifndef LCD_PORT_NAME_D5
        #error "LCD_PORT_NAME_D5 not defined."
    #endif

    #ifndef LCD_PIN_NAME_D5
        #error "LCD_PIN_NAME_D5 not defined."
    #endif

    #ifndef LCD_PIN_D5
        #error "LCD_PIN_D5 not defined."
    #endif
    
    #ifndef LCD_DDR_NAME_D6
        #error "LCD_DDR_NAME_D6 not defined."
    #endif

    #ifndef LCD_PORT_NAME_D6
        #error "LCD_PORT_NAME_D6 not defined."
    #endif

    #ifndef LCD_PIN_NAME_D6
        #error "LCD_PIN_NAME_D6 not defined."
    #endif

    #ifndef LCD_PIN_D6
        #error "LCD_PIN_D6 not defined."
    #endif
    
    #ifndef LCD_DDR_NAME_D7
        #error "LCD_DDR_NAME_D7 not defined."
    #endif

    #ifndef LCD_PORT_NAME_D7
        #error "LCD_PORT_NAME_D7 not defined."
    #endif

    #ifndef LCD_PIN_NAME_D7
        #error "LCD_PIN_NAME_D7 not defined."
    #endif

    #ifndef LCD_PIN_D7
        #error "LCD_PIN_D7 not defined."
    #endif
    
#endif

#ifdef LCD_DATA_LENGTH_8BIT
    #ifndef LCD_DDR_D0D7
        #error "LCD_DDR_D0D7 not defined."
    #endif

    #ifndef LCD_PORT_D0D7
        #error "LCD_PORT_D0D7 not defined."
    #endif
#endif

/**
 * Класс CLCD
 * ~~~~~~~~~~
 *
 * Назначение:
 * Обёртка для работы с а/ц ЖКИ.
 *
 * Автор: Мезенцев Вячеслав
 *
 * Почта: mailto:unihomelab@ya.ru
 *
 * ВНИМАНИЕ:
 * Этот файл формируется автоматически в среде EA.
 * Если Вы что-то изменили здесь, то синхронизируйте изменения в EA.
 */
class CLCD {

public:

	/**
	 * Тип передаваемыз данных
	 */
	enum EnLCDDataType
	{
		/**
		 * Команда
		 */
		TYPE_COMMAND = 0,
		/**
		 * Данные
		 */
		TYPE_DATA = 1
	};

	/**
	 * Перечисление поддерживаемых LCD индикаторов
	 */
	enum EnLCDDriverType
	{
		/**
		 * Драйвер LCD дисплея не выбран
		 */
		LCD_DRIVER_NONE = 0,
		/**
		 * Контроллер HD44780 фирмы Hitachi
		 */
		LCD_DRIVER_HD44780 = 1,
		/**
		 * Контроллер пользователя
		 */
		LCD_DRIVER_USER = 2
	};	   

	static void Initialization(EnLCDDriverType DriverType = LCD_DRIVER_NONE);
	static void SetCols(uint8_t Cols);
	static void SetRows(uint8_t Rows);
    static void Clear();
	static void Home();
	static void ShiftLeft();
	static void ShiftRight();
	static void Goto(uint8_t Row, uint8_t Col);
    static void HideCursor( bool Blinking = false );
    static void ShowCursor( bool Blinking = false );
    static void ShowCursor( bool Blinking, uint8_t Row, uint8_t Col );
    static void MoveCursorLeft( uint8_t Count );
    static void MoveCursorRight( uint8_t Count );
    static void ShowProgressBar( uint8_t Progress, uint8_t MaxProgress, uint8_t Length );
    static void ShowProgressBar( uint8_t Progress, uint8_t MaxProgress, uint8_t Length, uint8_t Row, uint8_t Col );
	static void PutChar(uint8_t Data);
    static void PutChar( uint8_t Data, uint8_t Row, uint8_t Col );
    static void LoadCustomChar(FU08T_PTR pCustomCharArray, uint8_t romCharNum, uint8_t lcdCharNum);
    static void LoadCustomChar(const uint8_t * pCustomCharArray, uint8_t romCharNum, uint8_t lcdCharNum);
    static void LoadCustomChars();
    static void Draw( uint8_t Buffer[], uint8_t Row, uint8_t Col );
    static void WriteLine(uint8_t Line, FCHAR_PTR pString);
    static void WriteLine(uint8_t Line, char * pString);
	static void WriteString( FCHAR_PTR pString, uint8_t Row, uint8_t Col );	
	static void WriteString( const char * pString, uint8_t Row, uint8_t Col );
	static void WriteString( FCHAR_PTR pString );	
	static void WriteString( const char * pString );

private:	/**
	 * Количество строк индикатора
	 */
	static uint8_t Rows;
	/**
	 * Количество столбцов индикатора
	 */
	static uint8_t Cols;
	/**
	 * Тип используемого драйвера
	 */
	static EnLCDDriverType Driver;	
	inline static void Init_HD44780();
	inline static void Init_User();
	inline static void WriteData(EnLCDDataType Type, uint8_t Data);
    inline static uint8_t ReadAddress();
    inline static uint8_t ReadData();
    inline static void Scroll(uint8_t Direction);
	inline static void SetLCDDriver(EnLCDDriverType DriverType);
	inline static bool IsBusy();
	inline static void WriteStrobe(uint16_t usec);
	inline static void WriteLow4bits(uint8_t data);
	inline static uint8_t ReadLow4bits();
	
};
