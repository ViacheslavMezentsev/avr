///////////////////////////////////////////////////////////
//  SystemTime.h
//  Implementation of the Class CSystemTime
//  Created on:      07-май-2012 1:36:54
//  Original author: Слава
///////////////////////////////////////////////////////////

#if !defined(EA_023C4557_1C97_4ae4_A696_FAF1364A36B8__INCLUDED_)
#define EA_023C4557_1C97_4ae4_A696_FAF1364A36B8__INCLUDED_

#define SECONDS_PER_DAY 86400UL
#define SECONDS_PER_HOUR 3600UL
#define MSECONDS_PER_SECOND 1000UL


class CSystemTime {

private:
    static uint32_t Counter;

    inline static bool IsLeapYear( uint16_t Year );
    inline static uint32_t DaysToMonth( uint16_t Year, uint8_t Month );
    inline static uint32_t DaysToYear( uint16_t Year );

public:

    static uint16_t GetYear();
    static uint8_t GetMonth();
    static uint8_t GetDay();
    static uint8_t GetHour();
    static uint8_t GetMinute();
    static uint8_t GetSecond();
    static uint16_t GetMilliSeconds();
    static uint8_t GetDayOfWeek();

    static uint32_t GetTime();
    static void SetTime( uint32_t Time );

    static time_t GetTimeAsUnixTime();
    static void SetTime( time_t & Time );

    static void GetTimeAsSystemTime( PSYSTEMTIME pSystemTime );
    static void SetTime( PSYSTEMTIME pSystemTime );

};
#endif // !defined(EA_023C4557_1C97_4ae4_A696_FAF1364A36B8__INCLUDED_)
