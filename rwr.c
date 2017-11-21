#include "rwr.h"
#include "interrupts.h"
#include "led.h"

/******
 *	Moduł do zapisywania wyjścia w kolejki ustalonej wielkości
 ******/

char bufferIn[WR_QUEUE_SIZE]; //Bufory do kolejek
char bufferOut[WR_QUEUE_SIZE];
Queue queue[2];
char tmp;
bool sending = false;

void wrPrepareUSARTDMAOUT() {
	DMA1_Stream6->CR = 4U << 25 |
		DMA_SxCR_PL_1 |
		DMA_SxCR_MINC |
		DMA_SxCR_DIR_0 |
		DMA_SxCR_TCIE;
		DMA1_Stream6->PAR = (uint32_t)&USART2->DR;
}

void wrPrepareUSARTDMAIN() {
	DMA1_Stream5->CR = 4U << 25 |
		DMA_SxCR_PL_1 |
		DMA_SxCR_MINC |
		DMA_SxCR_TCIE;
		DMA1_Stream5->PAR = (uint32_t)&USART2->DR;
}

void wrPrepareUSART() {
	GPIOafConfigure(GPIOA, 2, GPIO_OType_PP, GPIO_Fast_Speed, GPIO_PuPd_NOPULL, GPIO_AF_USART2);
	GPIOafConfigure(GPIOA, 3, GPIO_OType_PP, GPIO_Fast_Speed, GPIO_PuPd_UP, GPIO_AF_USART2);
	USART2->CR1 = USART_Mode_Rx_Tx | USART_WordLength_8b | USART_Parity_No;
	USART2->CR2 = USART_StopBits_1;
	uint32_t const baudrate = 9600U;
	USART2->BRR = (PCLK1_HZ + (baudrate / 2U)) / baudrate;
	USART2->CR3 = USART_CR3_DMAT | USART_CR3_DMAR;
	
	wrPrepareUSARTDMAOUT();
	wrPrepareUSARTDMAIN();
	DMA1->HIFCR = DMA_HIFCR_CTCIF6 | DMA_HIFCR_CTCIF5;
}

void wrPrepareHandlers() {
	INT_HANDLER outHandler;
	outHandler.checkBit = DMA_HISR_TCIF6;
	outHandler.handleBit = DMA_HIFCR_CTCIF6;
	outHandler.data = 0;
	outHandler.function = &wrProcSend;
	intAddHandler(INT_DMA1S6, outHandler);
	IRQsetPriority(DMA1_Stream6_IRQn, LOW_IRQ_PRIO, VERY_HIGH_IRQ_SUBPRIO);
	NVIC_EnableIRQ(DMA1_Stream6_IRQn);
	//NVIC_EnableIRQ(DMA1_Stream5_IRQn);
}

void wrPrepare() {
	//Przygotowujemy kolejki
	queueInit(&queue[WR_QUEUE_IN], bufferIn, WR_QUEUE_SIZE);
	queueInit(&queue[WR_QUEUE_OUT], bufferOut, WR_QUEUE_SIZE);
	//Konfigurujemy po czym włączamy USART
	wrPrepareUSART();
	wrPrepareHandlers();
	USART2->CR1 |= USART_Enable;
}

Queue* getQueue(WR_QUEUE_TYPE num) {
	return &queue[num];
}

void wrStartSend() {
	uint32_t level = IRQprotectAll();
	if (!queueEmpty(&queue[WR_QUEUE_OUT])) {
		sending = true;
		tmp = queuePeek(&queue[WR_QUEUE_OUT]);
		DMA1_Stream6->M0AR = (uint32_t) (&tmp);
		DMA1_Stream6->NDTR = 1;
		DMA1_Stream6->CR |= DMA_SxCR_EN;
		queuePop(&queue[WR_QUEUE_OUT]);
	} else {
		sending = false;
	}
	/*else if (!(DMA1_Stream6->CR & DMA_SxCR_EN) && !empty) {
		tmp = '&';
		DMA1_Stream6->M0AR = (uint32_t) (&tmp);
		DMA1_Stream6->NDTR = 1;
		DMA1_Stream6->CR |= DMA_SxCR_EN;
		empty = true;
	}
	*/
	IRQunprotectAll(level);
}

void wrSend(char *string) {
	uint32_t level = IRQprotectAll();
	queuePutStr(&queue[WR_QUEUE_OUT], string);
	if (!sending) {
		wrStartSend();
	}
	IRQunprotectAll(level);
}

void wrProcSend(void *data) {
	wrStartSend();
}
