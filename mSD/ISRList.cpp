#include "Defines.h"
#include "Configuration.h"
#include "MCU.h"

// -=[ Процедуры обработки прерываний ] [ATmega32]=-

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
 * External Interrupt Request 2
 */
#ifdef External_Interrupt_Request_2

    #ifdef __GNUC__

        ISR( INT2_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = INT2_vect
        __interrupt void ExternalInterruptRequest2() {

    #endif

        CMCU::OnExternalInterruptRequest2();

    } // External Interrupt Request 2

#endif

/**
 * Timer/Counter2 Compare Match
 */
#ifdef Timer_Counter2_Compare_Match

    #ifdef __GNUC__

        ISR( TIMER2_COMP_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = TIMER2_COMP_vect
        __interrupt void TimerCounter2CompareMatch() {

    #endif

        CMCU::OnTimerCounter2CompareMatch();

} // Timer/Counter2 Compare Match

#endif

/**
 *  Timer/Counter2 Overflow
 */
#ifdef Timer_Counter2_Overflow

    #ifdef __GNUC__

        ISR( TIMER2_OVF_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = TIMER2_OVF_vect
        __interrupt void TimerCounter2Overflow() {

    #endif

        CMCU::OnTimerCounter2Overflow();

    } // Timer/Counter2 Overflow

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
 *  Timer/Counter1 Overflow
 */
#ifdef Timer_Counter1_Overflow

    #ifdef __GNUC__

        ISR( TIMER1_OVF_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = TIMER1_OVF_vect
        __interrupt void TimerCounter1Overflow() {

    #endif

        CMCU::OnTimerCounter1Overflow();

    } // Timer/Counter1 Overflow

#endif

/**
 *  Timer/Counter0 Compare Match
 */
#ifdef Timer_Counter0_Compare_Match

    #ifdef __GNUC__

        ISR( TIMER0_COMP_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = TIMER0_COMP_vect
        __interrupt void TimerCounter0CompareMatch() {

    #endif

        CMCU::OnTimerCounter0CompareMatch();

    } // Timer/Counter0 Compare Match

#endif

/**
 *  Timer/Counter0 Overflow
 */
#ifdef Timer_Counter0_Overflow

    #ifdef __GNUC__

        ISR( TIMER0_OVF_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = TIMER0_OVF_vect
        __interrupt void TimerCounter0Overflow() {

    #endif

        CMCU::OnTimerCounter0Overflow();

    } // Timer/Counter0 Overflow

#endif

/**
 *  SPI Serial Transfer Complete
 */
#ifdef SPI_Serial_Transfer_Complete

    #ifdef __GNUC__

        ISR( SPI_STC_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = SPI_STC_vect
        __interrupt void SPISerialTransferComplete() {

    #endif

        CMCU::OnSPISerialTransferComplete();

    } // SPI Serial Transfer Complete

#endif

/**
 *  USART, Rx Complete
 */
#ifdef USART_Rx_Complete

    #ifdef __GNUC__

        ISR( USART_RXC_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = USART_RXC_vect
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

        #pragma vector = USART_UDRE_vect
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

        #pragma vector = USART_TXC_vect
        __interrupt void USARTTxComplete() {

    #endif

        CMCU::OnUSARTTxComplete();

    } // USART, Tx Complete

#endif

/**
 *  ADC Conversion Complete
 */
#ifdef ADC_Conversion_Complete

    #ifdef __GNUC__

        ISR( ADC_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = ADC_vect
        __interrupt void ADCConversionComplete() {

    #endif

        CMCU::OnADCConversionComplete();

    } // ADC Conversion Complete

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
 *  2-wire Serial Interface
 */
#ifdef TWI_Serial_Interface

    #ifdef __GNUC__

        ISR( TWI_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = TWI_vect
        __interrupt void TWISerialInterface() {

    #endif

        CMCU::OnTWISerialInterface();

    } // 2-wire Serial Interface

#endif

/**
 *  Store Program Memory Read
 */
#ifdef Store_Program_Memory_Read

    #ifdef __GNUC__

        ISR( SPM_RDY_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = SPM_RDY_vect
        __interrupt void StoreProgramMemoryRead() {

    #endif

        CMCU::OnStoreProgramMemoryRead();

    } // Store Program Memory Read

#endif
