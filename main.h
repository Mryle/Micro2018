#include <delay.h>
#include <gpio.h>
#include <stm32.h>
#include <string.h>
#include <irq.h>

#include "led.h"	// Obsługa ledów
#include "tim.h"	// Obsługa timerów (do uproszczenia)
#include "key.h"	// Obsługa klawiatury
#include "queue.h"	// Obsługa kolejek

#include <lcd.h>
