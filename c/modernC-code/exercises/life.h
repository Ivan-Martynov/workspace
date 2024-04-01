#ifndef _H_LIFE_H_
#define _H_LIFE_H_

#include <stdbool.h>
#include <stdatomic.h>

_Atomic(size_t) constellations;
_Atomic(size_t) x0;
_Atomic(size_t) x1;
_Atomic(size_t) frames;
_Atomic(bool) finished;

#endif // _H_LIFE_H_
