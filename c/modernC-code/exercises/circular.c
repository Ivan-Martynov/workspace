#include <stdlib.h>
#include <string.h>

#include "circular.h"

struct circular
{
    size_t start;
    size_t len;
    size_t max_len;
    double* tab;
};

circular* circular_init(circular* c, size_t max_len)
{
    if (c)
    {
        if (max_len)
        {
            *c = (circular) {
                .max_len = max_len,
                .tab = malloc(sizeof(double[max_len])),
            };

            // Allocation failed.
            if (!c->tab)
            {
                c->max_len = 0;
            }
        }
        else
        {
            *c = (circular) {0};
        }
    }

    return c;
}

void circular_destroy(circular* c)
{
    if (c)
    {
        free(c->tab);
        circular_init(c, 0);
    }
}

circular* circular_new(size_t len)
{
    circular* c = malloc(sizeof(*c));
    return circular_init(c, len);
}

void circular_delete(circular* c)
{
    circular_destroy(c);
    if (c)
    {
        free(c);
    }
}

circular* circular_append(circular* c, double value)
{
    if (c)
    {
        double* target_ptr = circular_element(c, c->len);
        if (target_ptr)
        {
            *target_ptr = value;
            ++c->len;
        }
    }

    return c;
}

double circular_pop(circular* c)
{
    double ret = 0.0;
    if (c && c->len)
    {
        double* target_ptr = circular_element(c, 0);

        if (target_ptr)
        {
            ret = *target_ptr;
        }

        ++c->start;
        --c->len;
    }
     
     return ret;
}

size_t circular_getlength(circular* c)
{
     return c ? c->len : 0;
}

static size_t circular_getpos(circular* c, size_t pos)
{
    pos += c->start;
    pos %= c->max_len;

    return pos;
}

double* circular_element(circular* c, size_t pos)
{
    double* ret = 0;
    if (c && (pos < c->max_len))
    {
        pos = circular_getpos(c, pos);
        ret = &c->tab[pos];
    }

    return ret;
}

circular* circular_resize(circular* c, size_t nlen)
{
    if (c)
    {
        size_t len = c->len;
        if (len > nlen)
        {
            return 0;
        }

        size_t olen = c->max_len;
        if (nlen != olen)
        {
            size_t ostart = circular_getpos(c, 0);
            size_t nstart = ostart;

            double* otab = c->tab;
            double* ntab;
            if (nlen > olen) // Enlarge.
            {
                ntab = realloc(c->tab, nlen * sizeof(*ntab));
                if (!ntab)
                {
                    return 0;
                }

                if (ostart + len > olen)
                {
                    size_t ulen = olen - ostart;
                    size_t llen = len - ulen;

                    if (llen <= (nlen - olen))
                    {
                        memcpy(ntab + olen, ntab, llen * sizeof(*ntab));
                    }
                    else
                    {
                        nstart = nlen - ulen;
                        memmove(
                            ntab + nstart, ntab + ostart, ulen * sizeof(*ntab));
                    }
                }
            }
            else // Shrink.
            {
                if (ostart + len > olen)
                {
                    size_t ulen = olen - ostart;
                    nstart = nlen - ulen;
                    memmove(otab + nstart, otab + ostart, ulen * sizeof(*ntab));
                }
                else if (ostart + len > nlen)
                {
                    memmove(otab, otab + ostart, len * sizeof(*ntab));
                    nstart = 0;
                }

                ntab = realloc(c->tab, nlen * sizeof(*ntab));
                if (!ntab)
                {
                    ntab = otab;
                }
            }

            *c = (circular) {
                .max_len = nlen,
                .start = nstart,
                .len = nlen,
                .tab = ntab,
            };
        }
    }

    return c;
}

void circular_fput(circular* c, FILE* s)
{
    if (c && c->tab)
    {
        const size_t len = circular_getlength(c);
        fprintf(
            s, "%p+%zu (%zu+%zu):", (void*)(c->tab), c->max_len, c->start, len);
        for (size_t i = c->start; i < len; ++i)
        {
            fprintf(s, "\t%g", *circular_element(c, i));
        }

        fprintf(s, "\n");
    }
    else
    {
        fputs("Invalid circular\n", s);
    }
}
