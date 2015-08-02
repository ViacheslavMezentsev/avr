///////////////////////////////////////////////////////////
//  MessagesQueue.h
//  Implementation of the Class CMessagesQueue
//  Created on:      19-июл-2011 13:37:20
//  Original author: Слава
///////////////////////////////////////////////////////////

#if !defined(EA_83780553_4E82_4cd8_925E_2BADA3A75F20__INCLUDED_)
#define EA_83780553_4E82_4cd8_925E_2BADA3A75F20__INCLUDED_

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
#endif // !defined(EA_83780553_4E82_4cd8_925E_2BADA3A75F20__INCLUDED_)
