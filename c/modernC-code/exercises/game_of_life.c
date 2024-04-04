#include "life.h"

#include <stdio.h>
#include <threads.h>

int draw_thread(void* L_ptr)
{
    life* restrict L = L_ptr;

    if (L->n0 < 30)
    {
        life_draw(L);
    }
    else
    {
        life_draw4(L);
    }

    L->drawn++;
}

void life_account(void* L_ptr)
{

}

int account_thread(void* L_ptr)
{
    life* restrict L = L_ptr;
    while (!L->finished)
    {
        // Blocks until there is work.
        life_account(L);

        if ((L->last + repetition) < L->accounted)
        {
            L->finished =true;
        }
    }

    return 0;
}

int main(void)
{
    // Create an object to hold game's data.
    life L = LIFE_INITIALIZER;
    life_init(&L, n0, n1, M);

    // Create threads to operate on the same object.
    thrd_t thrd[4];
    thrd_create(&thrd[0], update_thread, &L);
    thrd_create(&thrd[1], draw_thread, &L);
    thrd_create(&thrd[2], inut_thread, &L);
    thrd_create(&thrd[3], account_thread, &L);

    // Wait for the update thread to terminate.
    thrd_join(thrd[0], 0);

    // Inform that the game is over.
    L.finished = true;
    ungetc('q', stdin);

    // Wait for the other threads.
    thrd_join(thrd[1], 0);
    thrd_join(thrd[2], 0);
    thrd_join(thrd[3], 0);

    return 0;
}
