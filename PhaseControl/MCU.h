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
 * Обёртка для работы с микроконтроллером ATtiny2313.
 * 
 * Автор: Мезенцев Вячеслав
 * 
 * Почта: mailto:unihomelab@ya.ru
 * 
 * ВНИМАНИЕ:
 * Этот файл формируется автоматически в среде EA.
 * Если Вы что-то изменили здесь, то синхронизируйте изменения в EA.
 */
class CMCU {

public:

	static void Initialization();
	static void OnExternalInterruptRequest0();
	static void OnExternalInterruptRequest1();
	static void OnTimerCounter1CaptureEvent();
	static void OnTimerCounter1CompareMatchA();
	static void OnTimerCounter1Overflow();
	static void OnTimerCounter0Overflow();
    static void OnUSARTRxComplete( uint8_t );
	static void OnUSARTDataRegisterEmpty();
	static void OnUSARTTxComplete();	
    static void OnAnalogComparator();
    static void OnPinChangeInterrupt();
    static void OnTimerCounter1CompareMatchB();
    static void OnTimerCounter0CompareMatchA();
    static void OnTimerCounter0CompareMatchB();
    static void OnUSIStartCondition();
    static void OnUSIOverflow();
	static void OnEEPROMReady();
    static void OnWatchdogTimerOverflow();
    static HRESULT MainThreadProcedure();

private:
	inline static void PortsInit();
	inline static void Timer0Init();
	inline static void Timer1Init();
	inline static void USIInit();
	inline static void USARTInit();
    inline static void AnalogComparatorInit();
	inline static void InternalWDTInit();
	inline static void ExternalInterruptsInit();
    inline static void PinChangeInterruptsInit();
	inline static void ControlRegistersInit();
	
};
#endif // !defined(EA_65BAC4E6_2352_4d1c_A473_BA968DAA6985__INCLUDED_)
