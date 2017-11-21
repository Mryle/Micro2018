#include "queue.h"

void queueInit(Queue *q, char *b, int s) {
	q->i = b;
	q->o = b;
	q->s = s;
	q->e = 0;
	q->b = b;
}

bool queueEmpty(Queue *q) {
	return q->e == 0;
}

bool queueFull(Queue *q) {
	return q->e == q->s;
}

int queueSize(Queue *q) {
	return q->e;
}

int queueMaxSize(Queue *q) {
	return q->s;
}

char queuePeek(Queue *q) {
	return *(q->o);
}

void _checkOverflow(Queue *q, char **p) {
	if (*p == (q->b + q->s)) {
		(*p) = q->b;
	}
}

void queuePop(Queue *q) {
	q->o++;
	_checkOverflow(q, &q->o);
	q->e--;
}

void queuePut(Queue *q, char c) {
	if (!queueFull(q)) {
		*(q->i) = c;
		q->i++;
		_checkOverflow(q, &(q->i));
		q->e++;
	}
}

void queuePutStr(Queue *q, char *s) {
	while( *s != 0) {
		queuePut(q, *s);
		s++;
	}
}
