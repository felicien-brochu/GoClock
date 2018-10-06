
#ifndef __base_h__
#define __base_h__

// Using types from avr
#include <avr/common.h>

// for malloc and free
#include <stdlib.h>

// declare reset function @ address 0
void (*resetFunc)(void) = 0;

#endif // ifndef __base_h__
