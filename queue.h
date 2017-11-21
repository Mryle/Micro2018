#include <stdbool.h>

/**
 * Moduł do obsługi kolejki na buforze obrotowym
 */

typedef struct {
	int s, e;		//size, elem
	char *i, *o;	//in, out
	char *b;		//buffer
} Queue;

/*
 * Inicjalizacja kolejki
 * q - wskaźnik do struktury którą mamy zainicjalizować
 * b - wskaźnik na bufor z którego struktura ma korzystać
 * s - wielkość tego bufora
 */
void	queueInit	(Queue *q, char *b, int s);

bool	queueEmpty(Queue *q);
bool	queueFull	(Queue *q);
int		queueSize	(Queue *q);
int		queueMaxSize(Queue *q);

char	queuePeek	(Queue *q);
void	queuePop	(Queue *q);
void	queuePut	(Queue *q, char c);
void	queuePutStr	(Queue *q, char* s);
