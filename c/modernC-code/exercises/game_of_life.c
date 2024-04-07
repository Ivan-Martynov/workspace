#include "life.h"

#include <stdio.h>
#include <threads.h>

int draw_thread(void* L_ptr)
{
    life* restrict L = L_ptr;

    size_t x0 = 0;
    size_t x1 = 0;

    fputs(ESC_CLEAR ESC_CLRSCR, stdout);

    while (!L->finished)
    {
        // Blocks until there is work.
        mtx_lock(&L->mtx);

        while (!L->finished && (L->iteration <= L->drawn) && (x0 == L->x0)
               && (x1 == L->x1))
        {
            life_wait(&L->draw, &L->mtx);
        }

        if (L->n0 < 30)
        {
            life_draw(L);
        }
        else
        {
            life_draw4(L);
        }
        L->drawn++;

        mtx_unlock(&L->mtx);

        x0 = L->x0;
        x1 = L->x1;

        // No need to draw too quickly.
        life_sleep(1.0 / 40);
    }

    return 0;
}

int life_wait(cnd_t* cnd_ptr, mtx_t* mtx_ptr)
{
    struct timespec now;
    timespec_get(&now, TIME_UTC);

    now.tv_sec += 1;

    return cnd_timedwait(cnd_ptr, mtx_ptr, &now);
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
        mtx_lock(&L->mtx);

        while (!L->finished && (L->accounted == L->iteration))
        {
            life_wait(&L->acco, &L->mtx);
        }

        life_account(L);

        if ((L->last + repetition) < L->accounted)
        {
            L->finished =true;
        }

        cnd_signal(&L->upda);
        mtx_unlock(&L->mtx);
    }

    return 0;
}

int input_thread(void* L_ptr)
{
    termin_unbuffered();

    life* restrict L = L_ptr;

    enum
    {
        len = 32
    };

    char commmand[len];
    do
    {
        int c = getchar();
        switch (c)
        {
            case GO_LEFT:
                life_advance(L, 0, -1);
                break;
            case GO_RIGHT:
                life_advance(L, 0, 1);
                break;
            case GO_UP:
                life_advance(L, -1, 0);
                break;
            case GO_DOWN:
                life_advance(L, 1, 0);
                break;
            case GO_HOME:
                L->x0 = 1;
                L->x1 = 1;
                break;
            case ESCAPE:
                ungetc(termin_translate(termin_read_esc(len, commmand)), stdin);
                continue;
            case '+':
                if (L->frames < 128)
                {
                    ++L->frames;
                }
                continue;
            case '-':
                if (L->frames > 1)
                {
                    --L->frames;
                }
                continue;
            case ' ':
                __attribute__((fallthrough));
            case 'b':
                __attribute__((fallthrough));
            case 'B':
                mtx_lock(&L->mtx);
                life_birth9(L);
                cnd_signal(&L->draw);
                mtx_unlock(&L->mtx);
                continue;
            case 'q':
                __attribute__((fallthrough));
            case 'Q':
                __attribute__((fallthrough));
            case EOF:
                goto FINISH_LABEL;
        }

        cnd_signal(&L->draw);
    } while (!(L->finished) || feof(stdin));

FINISH_LABEL:
    L->finished = true;
    return 0;
}

int update_thread(void* L_ptr)
{
    life* restrict L = L_ptr;

    size_t changed = 1;
    size_t birth9 = 0;
    while (!L->finished && changed)
    {
        // Blocks until there is work.
        mtx_lock(&L->mtx);
        while (!L->finished && (L->accounted < L->iteration))
        {
            life_wait(&L->upda, &L->mtx);
        }

        if (birth9 != L->birth9)
        {
            life_torus(L);
        }

        life_count(L);
        changed = life_update(L);
        life_torus(L);
        birth9 = L->birth9;
        L->iteration++;

        cnd_signal(&L->acco);
        cnd_signal(&L->draw);
        mtx_unlock(&L->mtx);

        life_sleep(1.0 / L->frames);
    }

    return 0;
}

void game_at_exit(void)
{
    // Puts the board in a nice final picture.
    L.iteration = L.last;
    life_draw(&L);
    life_destroy(&L);
}

int main(int argc, char* argv[argc + 1])
{
    size_t n0 = 30;
    size_t n1 = 80;

    if (argc > 1)
    {
        n0 = strtoull(argv[1], 0, 0);
    }

    if (argc > 2)
    {
        n1 = strtoull(argv[2], 0, 0);
    }

    // Create an object to hold game's data.
    life L = LIFE_INITIALIZER;
    life_init(&L, n0, n1, M);

    atexit(game_at_exit);

    // Create threads to operate on the same object.
    thrd_t thrd[4];
    thrd_create(&thrd[0], update_thread, &L);
    thrd_create(&thrd[1], draw_thread, &L);
    thrd_create(&thrd[2], input_thread, &L);
    thrd_create(&thrd[3], account_thread, &L);

#if 0
    // Create threads to operate on the same object and discard their ids.
    thrd_create(&(thrd_t) {0}, update_thread, &L);
    thrd_create(&(thrd_t) {0}, draw_thread, &L);
    thrd_create(&(thrd_t) {0}, input_thread, &L);
#endif

    // Wait for the update thread to terminate.
    thrd_join(thrd[0], 0);

    // Inform that the game is over.
    L.finished = true;
    ungetc('q', stdin);

    // Wait for the other threads.
    thrd_join(thrd[1], 0);
    thrd_join(thrd[2], 0);
    thrd_join(thrd[3], 0);

    // End this thread nicely and let other threads go on nicely.
    thrd_exit(0);

    return 0;
}
