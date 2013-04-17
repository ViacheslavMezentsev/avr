///////////////////////////////////////////////////////////
//  MessagesQueue.cpp
//  Implementation of the Class CMessagesQueue
//  Created on:      19-июл-2011 13:37:20
//  Original author: Слава
///////////////////////////////////////////////////////////

#include "Defines.h"
#include "Configuration.h"
#include "MessagesQueue.h"


MSG EmptyMsg = {
    NULL, // hwnd
    NULL, // message
    0,    // wParam
    0,    // lParam
    0     // time
};


CMessagesQueue::CMessagesQueue(){

    m_Head = 0;
    m_Tail = 0;
    m_LenBuffer = MESSAGE_QUEUE_LENGTH;

}


void CMessagesQueue::get(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax){

    // Если очередь пустая, то посылаем условно-пустое сообщение
    if ( empty() ) {

        * lpMsg = EmptyMsg;

        return;
    }

    * lpMsg = m_Buffer[ m_Tail ];

    m_Tail++;

    if ( m_Tail > ( m_LenBuffer - 1 ) ) m_Tail = 0;

}


void CMessagesQueue::put(LPMSG lpMsg){

    if ( space() > 0 ) {

        m_Buffer[ m_Head ] = * lpMsg;

        m_Head++;

        if ( m_Head > ( m_LenBuffer - 1 ) ) m_Head = 0;

    }

    // Иначе сообщение не попадает в очередь сообщений и пропадает
}


// Хвост начинается с головы
bool CMessagesQueue::empty(){

	return ( m_Head == m_Tail );

}


// !space() - нет места, т.е. заполнена
bool CMessagesQueue::filled(){

	return ( !space() );

}


uint16_t CMessagesQueue::length(){

    if ( m_Head >  m_Tail ) {

        return ( m_Head - m_Tail );

    } else {

        if ( m_Head < m_Tail ) {

            return ( m_LenBuffer - ( m_Tail - m_Head ) );

        } else {

            return 0;

        }

    }

}


uint16_t CMessagesQueue::space(){

	return (  m_LenBuffer - length() );

}

