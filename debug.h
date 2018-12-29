#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "usart.h"

static FILE usartout = FDEV_SETUP_STREAM (put, get, _FDEV_SETUP_RW);
#define DEBUG(...) fprintf (&usartout, __VA_ARGS__)

#endif /*_DEBUG_H_*/
