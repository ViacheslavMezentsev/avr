///////////////////////////////////////////////////////////
//  SystemTime.cpp
//  Implementation of the Class CSystemTime
//  Created on:      07-май-2012 1:36:55
//  Original author: Слава
///////////////////////////////////////////////////////////

#include "Defines.h"
#include "Configuration.h"
#include "SystemTime.h"


// -=[ Постоянные во флеш-памяти ]=-

#ifdef __ICCAVR__
    __flash uint16_t DoM[12] = {
#elif defined __GNUC__
    const PROGMEM uint16_t _DoM[12] = {
#endif
        0, 31U, 59U, 90U, 120U, 151U, 181U, 212U, 243U, 273U, 304U, 334U
};
#ifdef __GNUC__
    FU16T_PTR DoM(_DoM);
#endif


//#ifdef __ICCAVR__
//    __flash uint16_t CommonYearMonthDays[12] = {
//#elif defined __GNUC__
//    PROGMEM uint16_t _CommonYearMonthDays[12] = {
//#endif
//        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
//};
//#ifdef __GNUC__
//    FU08T_PTR CommonYearMonthDays(_CommonYearMonthDays);
//#endif
//
//
//#ifdef __ICCAVR__
//    __flash uint16_t LeapYearMonthDays[12] = {
//#elif defined __GNUC__
//    PROGMEM uint16_t _LeapYearMonthDays[12] = {
//#endif
//        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
//};
//#ifdef __GNUC__
//    FU08T_PTR LeapYearMonthDays(_LeapYearMonthDays);
//#endif

//Constants
//
//RFC2822_MONTH_NAME = [ 'Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec' ]
//
//RFC2822_DAY_NAME = [ 'Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat' ]
//
//MonthValue = { 'JAN' => 1, 'FEB' => 2, 'MAR' => 3, 'APR' => 4, 'MAY' => 5, 'JUN' => 6, 'JUL' => 7, 'AUG' => 8, 'SEP' => 9, 'OCT' =>10, 'NOV' =>11, 'DEC' =>12 }
//
//CommonYearMonthDays = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]
//
//LeapYearMonthDays = [31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]
//
//ZoneOffset = { 'UTC' => 0, # ISO 8601 'Z' => 0, # RFC 822 'UT' => 0, 'GMT' => 0, 'EST' => -5, 'EDT' => -4, 'CST' => -6, 'CDT' => -5, 'MST' => -7, 'MDT' => -6, 'PST' => -8, 'PDT' => -7, # Following definition of military zones is original one. # See RFC 1123 and RFC 2822 for the error in RFC 822. 'A' => +1, 'B' => +2, 'C' => +3, 'D' => +4, 'E' => +5, 'F' => +6, 'G' => +7, 'H' => +8, 'I' => +9, 'K' => +10, 'L' => +11, 'M' => +12, 'N' => -1, 'O' => -2, 'P' => -3, 'Q' => -4, 'R' => -5, 'S' => -6, 'T' => -7, 'U' => -8, 'V' => -9, 'W' => -10, 'X' => -11, 'Y' => -12, }

// -=[ Переменные в ОЗУ ]=-

uint8_t Month, Day, Hours, Minutes, Seconds, DayOfWeek;
uint16_t Year, mSeconds;
uint32_t uiDate, uiDay, days_to_year, days_to_month;
uint32_t uiTime;

SYSTEMTIME Time;


// Обязательно инициализируем начальными значениями статические
// поля класса.

uint32_t CSystemTime::Counter = 0;


/***********************
*  Р Е А Л И З А Ц И Я
*  ~~~~~~~~~~~~~~~~~~~ 
************************/


bool CSystemTime::IsLeapYear( uint16_t Year ) {

    return ( ( ( ( Year % 400U == 0 ) || ( Year % 4U == 0 ) ) && ( Year % 100U != 0 ) ) ? true : false );

}


uint32_t CSystemTime::DaysToMonth( uint16_t Year, uint8_t Month ) {

    return ( uint32_t ) ( ( ( Month > 2 ) && IsLeapYear( Year ) ) ? DoM[ Month - 1 ] + 1 : DoM[ Month - 1 ] );

}
    
    
uint32_t CSystemTime::DaysToYear( uint16_t Year ) {

    // Убираем годы до начала отсчёта.
    uint32_t year = ( uint32_t ) Year - 1970UL;

    // Определяем количество дней в годах.
    return year * 365UL + ( year + 1UL ) / 4UL + ( year + 69UL ) / 100UL + ( year + 369UL ) / 400UL;
    
}


uint16_t CSystemTime::GetYear() {

    GetTimeAsSystemTime( & Time );

    return ( uint16_t ) Time.wYear;

}


uint8_t CSystemTime::GetMonth() {

    GetTimeAsSystemTime( & Time );

    return ( uint8_t ) Time.wMonth;

}


uint8_t CSystemTime::GetDay() {

    GetTimeAsSystemTime( & Time );

    return ( uint8_t ) Time.wDay;    

}


uint8_t CSystemTime::GetHour() {

    GetTimeAsSystemTime( & Time );

    return ( uint8_t ) Time.wHour; 

}


uint8_t CSystemTime::GetMinute() {

    GetTimeAsSystemTime( & Time );

    return ( uint8_t ) Time.wMinute; 

}


uint8_t CSystemTime::GetSecond() {

    GetTimeAsSystemTime( & Time );

    return ( uint8_t ) Time.wSecond; 

}


uint16_t CSystemTime::GetMilliSeconds() {

    GetTimeAsSystemTime( & Time );

    return ( uint8_t ) Time.wMilliseconds; 

}


uint8_t CSystemTime::GetDayOfWeek() {

    GetTimeAsSystemTime( & Time );

    return ( uint8_t ) Time.wDayOfWeek; 

}


uint32_t CSystemTime::GetTime() {

    return Counter;

}


void CSystemTime::SetTime( uint32_t Time ) {

    Counter = Time;

}


time_t CSystemTime::GetTimeAsUnixTime() {

    return ( time_t ) Counter;

}
    
    
void CSystemTime::SetTime( time_t & Time ) {

    Counter = ( uint32_t ) Time;

}


void CSystemTime::GetTimeAsSystemTime( PSYSTEMTIME pSystemTime ) {

    // Переводим количество миллисекунд в количество дней с 1970 г.
    uiDate = Counter / SECONDS_PER_DAY;

    // День недели, считая от нуля.
    DayOfWeek = ( uint8_t ) ( ( uiDate + 3UL ) % 7UL );

    // Грубо оцениваем год.
    Year = ( uint16_t ) ( uiDate / 365UL + 1970UL );

    // Определяем количество дней в истекших годах.
    days_to_year = DaysToYear( Year );

    // Делаем уточнение.
    if ( uiDate < days_to_year ) {
        
        Year =- 1;
        days_to_year = DaysToYear( Year );

    }

    // Определяем день года.
    uiDay = uiDate - days_to_year + 1;

    // Грубо оценим месяц.
    Month = ( uint8_t ) min( uiDay / 29UL + 1UL, 12 ); 

    // Определяем количество дней в истекших месяцах.
    days_to_month = DaysToMonth( Year, Month );

    // Делаем уточнение.
    if ( uiDay <= days_to_month ) {
        
        Month =- 1;
        days_to_month = DaysToMonth( Year, Month );

    }

    // Определяем день.
    Day = ( uint8_t ) ( uiDay - days_to_month );

    // Вычисляем эквивалент неполного дня в секундах.
    mSeconds = 0;

    uiTime = Counter % SECONDS_PER_DAY;

    Seconds = ( uint8_t ) ( uiTime % 60UL );

    uiTime /= 60UL;

    Minutes = ( uint8_t ) ( uiTime % 60UL );

    Hours = ( uint8_t ) ( uiTime / 60UL );

    pSystemTime->wYear = Year;
    pSystemTime->wMonth = Month;
    pSystemTime->wDayOfWeek = DayOfWeek;
    pSystemTime->wDay = Day;
    pSystemTime->wHour = Hours;
    pSystemTime->wMinute = Minutes;
    pSystemTime->wSecond = Seconds;
    pSystemTime->wMilliseconds = mSeconds;

}


void CSystemTime::SetTime( PSYSTEMTIME pSystemTime ) {

    // Преобразуем дату в uint32_t.
    uint32_t sec = 0;

    // Преобразуем дату.
    sec += DaysToYear( pSystemTime->wYear ) + DaysToMonth( pSystemTime->wYear, pSystemTime->wMonth ) + pSystemTime->wDay - 1;
    sec *= SECONDS_PER_DAY;

    // Преобразуем время.
    sec += ( ( uint32_t ) pSystemTime->wHour * 60UL + ( uint32_t ) pSystemTime->wMinute ) * 60UL + ( uint32_t ) pSystemTime->wSecond;

    // Записываем результат.
    Counter = sec;

}

