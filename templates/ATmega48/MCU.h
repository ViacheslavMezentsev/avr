///////////////////////////////////////////////////////////
//  MCU.h
//  Implementation of the Class CMCU
//  Created on:      14-июл-2011 11:34:33
//  Original author: Слава
///////////////////////////////////////////////////////////

#if !defined(EA_65BAC4E6_2352_4d1c_A473_BA968DAA6985__INCLUDED_)
#define EA_65BAC4E6_2352_4d1c_A473_BA968DAA6985__INCLUDED_

/**
 * Класс CMCU
 * ~~~~~~~~~~
 * 
 * Назначение:
 * Обёртка для работы с микроконтроллером ATmega48.
 * 
 * Автор: Мезенцев Вячеслав
 * 
 * Почта: mailto:unihomelab@ya.ru
 * 
 * ВНИМАНИЕ:
 * Этот файл формируется автоматически в среде EA.
 * Если Вы что-то изменили здесь, то синхронизируйте изменения в EA.
 */
class CMCU
{

public:

	inline static void OnExternalInterruptRequest0();
	inline static void OnExternalInterruptRequest1();
	inline static void OnPinChangeInterruptRequest0();
	inline static void OnPinChangeInterruptRequest1();
	inline static void OnPinChangeInterruptRequest2();
    inline static void OnWatchdogTimeoutInterrupt();
	inline static void OnTimerCounter2CompareMatchA();
	inline static void OnTimerCounter2CompareMatchB();
	inline static void OnTimerCounter2Overflow();
	inline static void OnTimerCounter1CaptureEvent();
	inline static void OnTimerCounter1CompareMatchA();
	inline static void OnTimerCounter1CompareMatchB();
	inline static void OnTimerCounter1Overflow();
	inline static void OnTimerCounter0CompareMatchA();
	inline static void OnTimerCounter0CompareMatchB();
	inline static void OnTimerCounter0Overflow();
	inline static void OnSPISerialTransferComplete();
	inline static void OnUSARTRxComplete( uint8_t data );
	inline static void OnUSARTDataRegisterEmpty();
	inline static void OnUSARTTxComplete();
	inline static void OnADCConversionComplete( uint16_t );
	inline static void OnEEPROMReady();
	inline static void OnAnalogComparator();
	inline static void OnTWISerialInterface();
	inline static void OnStoreProgramMemoryRead();
	
    static void Initialization();
    static HRESULT MainThreadProcedure();
	
private:
	inline static void PortsInit();
	inline static void ADCInit();
	inline static void Timer0Init();
	inline static void Timer1Init();
	inline static void Timer2Init();
	inline static void SPIInit();
	inline static void TWIInit();
	inline static void USARTInit();	
	inline static void InternalWDTInit();
	inline static void ExternalInterruptsInit();
	inline static void ControlRegistersInit();

};
#endif // !defined(EA_65BAC4E6_2352_4d1c_A473_BA968DAA6985__INCLUDED_)
