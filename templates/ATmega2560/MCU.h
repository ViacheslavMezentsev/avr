#pragma once

/**
 * ????? CMCU
 * ~~~~~~~~~~
 * 
 * ??????????:
 * ??????? ??? ?????? ? ????????????????? ATmega2560.
 * 
 * ?????: ???????? ????????
 * 
 * ?????: mailto:viacheslavmezentsev@ya.ru
 * 
 */
class CMCU
{

public:

    static void Initialization();
    static HRESULT MainThreadProcedure();

	static void OnExternalInterruptRequest0();
	static void OnExternalInterruptRequest1();
	static void OnExternalInterruptRequest2();
	static void OnExternalInterruptRequest3();
	static void OnExternalInterruptRequest4();
	static void OnExternalInterruptRequest5();
	static void OnExternalInterruptRequest6();
	static void OnExternalInterruptRequest7();
    
    static void OnPinChangeInterruptRequest0();
    static void OnPinChangeInterruptRequest1();
    static void OnPinChangeInterruptRequest2();
    
    static void OnWatchdogTimeoutInterrupt();
    
    static void OnTimerCounter2CompareMatchA();
    static void OnTimerCounter2CompareMatchB();   
	static void OnTimerCounter2Overflow();
    
	static void OnTimerCounter1CaptureEvent();
	static void OnTimerCounter1CompareMatchA();
	static void OnTimerCounter1CompareMatchB();
    static void OnTimerCounter1CompareMatchC();    
	static void OnTimerCounter1Overflow();
    
	static void OnTimerCounter0CompareMatchA();
    static void OnTimerCounter0CompareMatchB();
	static void OnTimerCounter0Overflow();
    
	static void OnSPISerialTransferComplete();
	
    static void OnUSART0RxComplete( uint8_t data );
	static void OnUSART0DataRegisterEmpty();
	static void OnUSART0TxComplete();
    
	static void OnAnalogComparator();
    
    static void OnADCConversionComplete( uint16_t );
    
	static void OnEEPROMReady();
	
	static void OnTimerCounter3CaptureEvent();
	static void OnTimerCounter3CompareMatchA();
	static void OnTimerCounter3CompareMatchB();
	static void OnTimerCounter3CompareMatchC();
	static void OnTimerCounter3Overflow();
    
	static void OnUSART1RxComplete( uint8_t data );
	static void OnUSART1DataRegisterEmpty();
	static void OnUSART1TxComplete();
    
	static void OnTWISerialInterface();

    static void OnStoreProgramMemoryRead();
    
	static void OnTimerCounter4CaptureEvent();
	static void OnTimerCounter4CompareMatchA();
	static void OnTimerCounter4CompareMatchB();
	static void OnTimerCounter4CompareMatchC();
	static void OnTimerCounter4Overflow();

	static void OnTimerCounter5CaptureEvent();
	static void OnTimerCounter5CompareMatchA();
	static void OnTimerCounter5CompareMatchB();
	static void OnTimerCounter5CompareMatchC();
	static void OnTimerCounter5Overflow();
    
    static void OnUSART2RxComplete( uint8_t data );
	static void OnUSART2DataRegisterEmpty();
	static void OnUSART2TxComplete();
    
    static void OnUSART3RxComplete( uint8_t data );
	static void OnUSART3DataRegisterEmpty();
	static void OnUSART3TxComplete();

private:
	static void ControlRegistersInit();
    static void PortsInit();
    static void Timer0Init();
    static void Timer1Init();
    static void Timer2Init();
    static void Timer3Init();
    static void Timer4Init();
    static void Timer5Init();
	static void SPIInit();
	static void TWIInit();
	static void USART0Init();
    static void USART1Init();
    static void USART2Init();
    static void USART3Init();
	static void InternalWDTInit();
	static void ExternalInterruptsInit();
	static void PinChangeInterruptsInit();
    static void ExternalSRAMInit();
    
};

