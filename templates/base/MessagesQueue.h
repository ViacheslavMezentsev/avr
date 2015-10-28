#pragma once

#include "Defines.h"

#ifndef MESSAGE_QUEUE_LENGTH
    #error "MESSAGE_QUEUE_LENGTH not defined."
#endif


/**
 * Класс для работы с очередью сообщений
 */
class CMessagesQueue
{

public:
	CMessagesQueue();

	bool empty();
	bool filled();
	void put(LPMSG lpMsg);
	void get(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);
	uint16_t length();
	
private:
	uint16_t m_Head;
	uint16_t m_Tail;
	uint16_t m_LenBuffer;
	MSG m_Buffer[ MESSAGE_QUEUE_LENGTH ];
	
	//inline uint16_t length();
	inline uint16_t space();

};
