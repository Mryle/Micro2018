#ifndef __KEY_H
#define __KEY_F

//#include "interrupts.h"
#include "tim.h"

/*
 * FYI
 * Klawiatura korzysta z przerwać EXTI 0-3 włączając i wyłączając je według własnych upodobań.
 */

typedef struct {
	int row;
	int column;
} KeyPress;

/**
 * Inicjalizacja klawiatury
 * tim - licznik z którego ma korzystać klawiatura.
 */
void keyPrepare();

/**
 * Handlery na czytanie klawiszy, do podpięcia pod odpowiednie
 **/
void keyRowHandler(uint32_t row);
/**
 * Handler na wykorzystywany drugi licznik
 **/
void keyTimerHandler();

#endif
