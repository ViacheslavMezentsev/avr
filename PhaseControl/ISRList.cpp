#include "Defines.h"
#include "Configuration.h"
#include "MCU.h"

// -=[ Внешние ссылки ]=-


// -=[ Процедуры обработки прерываний ] [ATtiny2313]=-

/**
* Внимание: При входе в обработчик прерывания флаг IE в SREG
* сбрасывается для запрещения вложенных прерываний.
*
* Для сохранения и восстановления SREG можно использовать
* следующие конструкции:
*
* // Сохраняем SREG
* uint8_t tmp = __save_interrupt();
*
* // ...
*
* // Восстанавливаем SREG
* __restore_interrupt( tmp );
*/


/**
 * External Interrupt Request 0
 */
#ifdef External_Interrupt_Request_0

    #ifdef __GNUC__

        ISR( INT0_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = INT0_vect
        __interrupt void ExternalInterruptRequest0() {

    #endif

        CMCU::OnExternalInterruptRequest0();

    } // External Interrupt Request 0

#endif


/**
 * External Interrupt Request 1
 */
#ifdef External_Interrupt_Request_1

    #ifdef __GNUC__

        ISR( INT1_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = INT1_vect
        __interrupt void ExternalInterruptRequest1() {

    #endif

        CMCU::OnExternalInterruptRequest1();

    } // External Interrupt Request 1

#endif


/**
 *  Timer/Counter1 Capture Event
 */
#ifdef Timer_Counter1_Capture_Event

    #ifdef __GNUC__

        ISR( TIMER1_CAPT_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = TIMER1_CAPT_vect
        __interrupt void TimerCounter1CaptureEvent() {

    #endif

        CMCU::OnTimerCounter1CaptureEvent();

    } // Timer/Counter1 Capture Event

#endif


/**
 *  Timer/Counter1 Compare Match A
 */
#ifdef Timer_Counter1_Compare_Match_A

    #ifdef __GNUC__

        ISR( TIMER1_COMPA_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = TIMER1_COMPA_vect
        __interrupt void TimerCounter1CompareMatchA() {

    #endif

        CMCU::OnTimerCounter1CompareMatchA();

    } // Timer/Counter1 Compare Match A

#endif


/**
 *  Timer/Counter1 Overflow
 */
#ifdef Timer_Counter1_Overflow

    #ifdef __GNUC__

        ISR( TIMER1_OVF_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = TIMER1_OVF1_vect
        __interrupt void TimerCounter1Overflow() {

    #endif

        CMCU::OnTimerCounter1Overflow();

    } // Timer/Counter1 Overflow

#endif


/**
 *  Timer/Counter0 Overflow
 */
#ifdef Timer_Counter0_Overflow

    #ifdef __GNUC__

        ISR( TIMER0_OVF_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = TIMER0_OVF0_vect
        __interrupt void TimerCounter0Overflow() {

    #endif

        CMCU::OnTimerCounter0Overflow();

    } // Timer/Counter0 Overflow

#endif


/**
 *  USART, Rx Complete
 */
#ifdef USART_Rx_Complete

    #ifdef __GNUC__

        ISR( USART_RX_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = USART0_RX_vect
        __interrupt void USARTRxComplete() {

    #endif

        CMCU::OnUSARTRxComplete( UDR );

    } // USART, Rx Complete

#endif


/**
 *  USART Data Register Empty
 */
#ifdef USART_Data_Register_Empty

    #ifdef __GNUC__

        ISR( USART_UDRE_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = USART0_UDRE_vect
        __interrupt void USARTDataRegisterEmpty() {

    #endif

        CMCU::OnUSARTDataRegisterEmpty();

    } // USART Data Register Empty

#endif


/**
 *  USART, Tx Complete
 */
#ifdef USART_Tx_Complete

    #ifdef __GNUC__

        ISR( USART_TXC_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = USART0_TX_vect
        __interrupt void USARTTxComplete() {

    #endif

        CMCU::OnUSARTTxComplete();

    } // USART, Tx Complete

#endif


/**
 *  Analog Comparator
 */
#ifdef Analog_Comparator

    #ifdef __GNUC__

        ISR( ANA_COMP_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = ANA_COMP_vect
        __interrupt void AnalogComparator() {

    #endif

        CMCU::OnAnalogComparator();

    } // Analog Comparator

#endif


/**
 *  Pin Change Interrupt
 */
#ifdef Pin_Change_Interrupt

    #ifdef __GNUC__

        ISR( PCINT_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = PCIN1_vect
        __interrupt void PinChangeInterrupt() {

    #endif

        CMCU::OnPinChangeInterrupt();

    } // Pin Change Interrupt

#endif


/**
 *  Timer/Counter1 Compare Match B
 */
#ifdef Timer_Counter1_Compare_Match_B

    #ifdef __GNUC__

        ISR( TIMER1_COMPB_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = TIMER1_COMPB_vect
        __interrupt void TimerCounter1CompareMatchB() {

    #endif

        CMCU::OnTimerCounter1CompareMatchB();

    } // Timer/Counter1 Compare Match B

#endif


/**
 *  Timer/Counter0 Compare Match A
 */
#ifdef Timer_Counter0_Compare_Match_A

    #ifdef __GNUC__

        ISR( TIMER0_COMPA_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = TIMER0_COMPA_vect
        __interrupt void TimerCounter0CompareMatchA() {

    #endif

        CMCU::OnTimerCounter0CompareMatchA();

    } // Timer/Counter0 Compare Match A

#endif


/**
 *  Timer/Counter0 Compare Match B
 */
#ifdef Timer_Counter0_Compare_Match_B

    #ifdef __GNUC__

        ISR( TIMER0_COMPB_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = TIMER0_COMPB_vect
        __interrupt void TimerCounter0CompareMatchB() {

    #endif

        CMCU::OnTimerCounter0CompareMatchB();

    } // Timer/Counter0 Compare Match B

#endif


/**
 *  USI Start Condition
 */
#ifdef USI_Start_Condition

    #ifdef __GNUC__

        ISR( USI_START_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = USI_STRT_vect
        __interrupt void USIStartCondition() {

    #endif

        CMCU::OnUSIStartCondition();

    } // USI Start Condition
#endif


/**
 *  USI Overflow
 */
#ifdef USI_Overflow

    #ifdef __GNUC__

        ISR( USI_OVERFLOW_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = USI_OVF_vect
        __interrupt void USIOverflow() {

    #endif

        CMCU::OnUSIOverflow();

    } // USI Overflow
#endif


/**
 *  EEPROM Ready
 */
#ifdef EEPROM_Ready

    #ifdef __GNUC__

        ISR( EE_RDY_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = EE_RDY_vect
        __interrupt void EEPROMReady() {

    #endif

        CMCU::OnEEPROMReady();

    } // EEPROM Ready
#endif


/**
 *  Watchdog Timer Overflow
 */
#ifdef Watchdog_Timer_Overflow

    #ifdef __GNUC__

        ISR( WDT_OVERFLOW_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = WDT_vect
        __interrupt void WatchdogTimerOverflow() {

    #endif

        CMCU::OnWatchdogTimerOverflow();

    } // Watchdog Timer Overflow
#endif









