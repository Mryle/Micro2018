#ifndef __RWR_H
#define __RWR_H

#include <gpio.h>
#include <stm32.h>
#include <irq.h>
#include "queue.h"

/*
 * Simple library to take care of asynchronous sending and receiving 
 * text messages from and sending it to terminal.
 */

typedef enum {
	WR_QUEUE_OUT	= 0,
	WR_QUEUE_IN		= 1
} WR_QUEUE_TYPE;

// Approximately number of characters that buffer holds
#define WR_QUEUE_SIZE 50

//USART Defines
#define USART_Mode_Rx_Tx (USART_CR1_RE | \
		USART_CR1_TE)
#define USART_Enable     USART_CR1_UE

#define USART_WordLength_8b 0x0000
#define USART_WordLength_9b USART_CR1_M

#define USART_Parity_No   0x0000
#define USART_Parity_Even USART_CR1_PCE
#define USART_Parity_Odd  (USART_CR1_PCE | \
		USART_CR1_PS)

#define USART_StopBits_1   0x0000
#define USART_StopBits_0_5 0x1000
#define USART_StopBits_2   0x2000
#define USART_StopBits_1_5 0x3000

#define USART_FlowControl_None 0x0000
#define USART_FlowControl_RTS  USART_CR3_RTSE
#define USART_FlowControl_CTS  USART_CR3_CTSE

#define HSI_HZ 16000000U
#define PCLK1_HZ HSI_HZ


void wrPrepare(); //Prepare Queues
Queue* getQueue(WR_QUEUE_TYPE num);
void wrProcSend();	//Automatically tries to send characters
void wrProcRead();	//Automatically reads character , resends and stores
void wrSend(char *string);

#endif
