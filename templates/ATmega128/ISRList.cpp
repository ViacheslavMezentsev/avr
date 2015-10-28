#include "Defines.h"
#include "Configuration.h"
#include "MCU.h"

// -=[ Внешние ссылки ]=-
 

// -=[ Процедуры обработки прерываний ] [ATmega128]=-

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
 * External Interrupt Request 3
 */
#ifdef External_Interrupt_Request_3

    #ifdef __GNUC__

        ISR( INT3_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = INT3_vect
        __interrupt void ExternalInterruptRequest3() {

    #endif

        CMCU::OnExternalInterruptRequest3();

    } // External Interrupt Request 3

#endif

/**
 * External Interrupt Request 4
 */
#ifdef External_Interrupt_Request_4

    #ifdef __GNUC__

        ISR( INT4_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = INT4_vect
        __interrupt void ExternalInterruptRequest4() {

    #endif
        
        CMCU::OnExternalInterruptRequest4();
        
    } // External Interrupt Request 4

#endif

/**
 * External Interrupt Request 5
 */
#ifdef External_Interrupt_Request_5

    #ifdef __GNUC__

        ISR( INT5_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = INT5_vect
        __interrupt void ExternalInterruptRequest5() {

    #endif

        CMCU::OnExternalInterruptRequest5();

    } // External Interrupt Request 5

#endif

/**
 * External Interrupt Request 6
 */
#ifdef External_Interrupt_Request_6

    #ifdef __GNUC__

        ISR( INT6_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = INT6_vect
        __interrupt void ExternalInterruptRequest6() {

    #endif

        CMCU::OnExternalInterruptRequest6();

    } // External Interrupt Request 6

#endif

/**
 * External Interrupt Request 7
 */
#ifdef External_Interrupt_Request_7

    #ifdef __GNUC__

        ISR( INT7_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = INT7_vect
        __interrupt void ExternalInterruptRequest7() {

    #endif
        
        CMCU::OnExternalInterruptRequest7();

    } // External Interrupt Request 7

#endif

/**
 * Timer/Counter2 Compare Match
 */
#ifdef External_Interrupt_Request_6

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
 *  USART0, Rx Complete
 */
#ifdef USART0_Rx_Complete

    #ifdef __GNUC__

        ISR( USART0_RX_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = USART0_RXC_vect
        __interrupt void USART0RxComplete() {

    #endif

        CMCU::OnUSART0RxComplete( UDR0 );

    } // USART0, Rx Complete

#endif

/**
 *  USART0 Data Register Empty
 */
#ifdef USART0_Data_Register_Empty

    #ifdef __GNUC__

        ISR( USART0_UDRE_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = USART0_UDRE_vect
        __interrupt void USART0DataRegisterEmpty() {

    #endif

        CMCU::OnUSART0DataRegisterEmpty();

    } // USART0 Data Register Empty

#endif

/**
 *  USART0, Tx Complete
 */
#ifdef USART0_Tx_Complete

    #ifdef __GNUC__

        ISR( USART0_TX_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = USART0_TXC_vect
        __interrupt void USART0TxComplete() {

    #endif

        CMCU::OnUSART0TxComplete();

    } // USART0, Tx Complete

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

        uint16_t temp = ADCL;
        temp += ( ADCH << 8 );

        CMCU::OnADCConversionComplete( temp );

    } // ADC Conversion Complete

#endif

/**
 *  EEPROM Ready
 */
#ifdef EEPROM_Ready

    #ifdef __GNUC__

        ISR( EE_READY_vect ) {

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

        ISR( ANALOG_COMP_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = ANA_COMP_vect
        __interrupt void AnalogComparator() {

    #endif

        CMCU::OnAnalogComparator();

    } // Analog Comparator

#endif

/**
 *  Timer/Counter1 Compare Match C
 */
#ifdef Timer_Counter1_Compare_Match_C

    #ifdef __GNUC__

        ISR( TIMER1_COMPC_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = TIMER1_COMPC_vect
        __interrupt void TimerCounter1CompareMatchC() {

    #endif

        CMCU::OnTimerCounter1CompareMatchC();

    } // Timer/Counter1 Compare Match C

#endif

/**
 *  Timer/Counter3 Capture Event
 */
#ifdef Timer_Counter3_Capture_Event

    #ifdef __GNUC__

        ISR( TIMER3_CAPT_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = TIMER3_CAPT_vect
        __interrupt void TimerCounter3CaptureEvent() {

    #endif

        CMCU::OnTimerCounter3CaptureEvent();

    } // Timer/Counter3 Capture Event

#endif

/**
 *  Timer/Counter3 Compare Match A
 */
#ifdef Timer_Counter3_Compare_Match_A

    #ifdef __GNUC__

        ISR( TIMER3_COMPA_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = TIMER3_COMPA_vect
        __interrupt void TimerCounter3CompareMatchA() {

    #endif

        CMCU::OnTimerCounter3CompareMatchA();

    } // Timer/Counter3 Compare Match A

#endif

/**
 *  Timer/Counter3 Compare Match B
 */
#ifdef Timer_Counter3_Compare_Match_B

    #ifdef __GNUC__

        ISR( TIMER3_COMPB_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = TIMER3_COMPB_vect
        __interrupt void TimerCounter3CompareMatchB() {

    #endif

        CMCU::OnTimerCounter3CompareMatchB();

    } // Timer/Counter3 Compare Match B

#endif

/**
 *  Timer/Counter3 Compare Match C
 */
#ifdef Timer_Counter3_Compare_Match_C

    #ifdef __GNUC__

        ISR( TIMER3_COMPC_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = TIMER3_COMPC_vect
        __interrupt void TimerCounter3CompareMatchC() {

    #endif

        CMCU::OnTimerCounter3CompareMatchC();

    } // Timer/Counter3 Compare Match C

#endif

/**
 *  Timer/Counter3 Overflow
 */
#ifdef Timer_Counter3_Overflow

    #ifdef __GNUC__

        ISR( TIMER3_OVF_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = TIMER3_OVF_vect
        __interrupt void TimerCounter3Overflow() {

    #endif

        CMCU::OnTimerCounter3Overflow();

    } // Timer/Counter3 Overflow

#endif

/**
 *  USART1, Rx Complete
 */
#ifdef USART1_Rx_Complete

    #ifdef __GNUC__

        ISR( USART1_RX_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = USART1_RXC_vect
        __interrupt void USART1RxComplete() {

    #endif

        CMCU::OnUSART1RxComplete( UDR1 );

    } // USART1, Rx Complete

#endif

/**
 *  USART1, Data Register Empty
 */
#ifdef USART1_Data_Register_Empty

    #ifdef __GNUC__

        ISR( USART1_UDRE_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = USART1_UDRE_vect
        __interrupt void USART1DataRegisterEmpty() {

    #endif

        CMCU::OnUSART1DataRegisterEmpty();

    } // USART1, Data Register Empty

#endif

/**
 *  USART1, Tx Complete
 */
#ifdef USART1_Tx_Complete

    #ifdef __GNUC__

        ISR( USART1_TX_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = USART1_TXC_vect
        __interrupt void USART1TxComplete() {

    #endif

        CMCU::OnUSART1TxComplete();

    } // USART1, Tx Complete

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

        ISR( SPM_READY_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = SPM_RDY_vect
        __interrupt void StoreProgramMemoryRead() {

    #endif

        CMCU::OnStoreProgramMemoryRead();

    } // Store Program Memory Read

#endif
