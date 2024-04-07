#ifndef _H_LIFE_H_
#define _H_LIFE_H_

#ifndef __STDC_NO_THREADS__
#include <threads.h>
#else
#error Needs C11 threads, aborting.
#endif

#include <stdbool.h>
#include <stdatomic.h>

enum
{
    life_maxit = 1ull << 23
};

typedef struct life life;
struct life
{
    mtx_t mtx;  // Mutex to protect Mv.
    cnd_t draw; // cnd to control drawing.
    cnd_t acco; // cnd to control accounting.
    cnd_t upda; // cnd to control updating.

    void* restrict Mv; // bool M[n0][n1];
    bool (*visited)[life_maxit]; // Hash constellations.

    size_t n0;
    size_t n1;
    size_t off0;
    size_t len0;
    size_t off1;
    size_t len1;
    void* restrict Bv;

    size_t iteration;
    size_t accounted;
    size_t drawn;
    size_t last;
    size_t birth9;

    _Atomic(size_t) constellations;
    _Atomic(size_t) x0;
    _Atomic(size_t) x1;
    _Atomic(size_t) frames;
    _Atomic(bool) finished;
};

#endif // _H_LIFE_H_
