#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "rat_num.h"

size_t gcd(size_t a, size_t b)
{
    const size_t r = a % b;
    return r ? gcd(b, r) : b;
}

rat rat_get(const long long num, const unsigned long long denom)
{
    rat ret
        = {.sign = (num < 0), .num = (num < 0) ? -num : num, .denom = denom};

    return ret;
}

rat rat_get_normal(rat x)
{
    const size_t c = gcd(x.num, x.denom);
    x.num /= c;
    x.denom /= c;
    return x;
}

rat rat_get_extended(rat x, size_t f)
{
    const size_t c = gcd(x.num, x.denom);
    x.num *= f;
    x.denom *= f;
    return x;
}

rat rat_get_prod(rat x, rat y)
{
    rat ret = {.sign = (x.sign != y.sign),
        .num = x.num * y.num,
        .denom = x.denom * y.denom};

    return rat_get_normal(ret);
}

rat rat_get_sum(rat x, rat y)
{
    const size_t c = gcd(x.denom, y.denom);
    const size_t ax = y.denom / c;
    const size_t bx = x.denom / c;

    x = rat_get_extended(x, ax);
    y = rat_get_extended(y, bx);

    assert(x.denom == y.denom);

    if (x.sign == y.sign)
    {
        x.num += y.num;
    }
    else if (x.num > y.num)
    {
        x.num -= y.num;
    }
    else
    {
        x.num = y.num - x.num;
        x.sign = !x.sign;
    }

    return rat_get_normal(x);
}

void rat_destroy(rat* rp)
{
    if (rp)
    {
        *rp = (rat) {0};
    }
}

rat* rat_init(rat* rp, long long num, unsigned long long denom)
{
    if (rp)
    {
        *rp = rat_get(num, denom);
    }

    return rp;
}

rat* rat_normalize(rat* rp)
{
    if (rp)
    {
        *rp = rat_get_normal(*rp);
    }

    return rp;
}

rat* rat_extend(rat* rp, size_t f)
{
    if (rp)
    {
        *rp = rat_get_extended(*rp, f);
    }

    return rp;
}

rat* rat_sumup(rat* rp, rat y)
{
    const size_t c = gcd(rp->denom, y.denom);
    const size_t ax = y.denom / c;
    const size_t bx = rp->denom / c;

    rat_extend(rp, ax);

    y = rat_get_extended(y, bx);

    assert(rp->denom == y.denom);

    if (rp->sign == y.sign)
    {
        rp->num += y.num;
    }
    else if (rp->num > y.num)
    {
        rp->num -= y.num;
    }
    else
    {
        rp->num = y.num - rp->num;
        rp->sign = !rp->sign;
    }

    return rat_normalize(rp);
}

rat* rat_rma(rat* rp, rat x, rat y)
{
    return rat_sumup(rp, rat_get_prod(x, y));
}

char const* rat_print(size_t len, char tmp[len], rat const* x)
{
    if (x->sign)
    {
        sprintf(tmp, "-%zu/%zu", x->num, x->denom);
    }
    else
    {
        sprintf(tmp, "+%zu/%zu", x->num, x->denom);
    }
}

rat rat_copy(const rat* const rat_ptr)
{
    return rat_get(
        rat_ptr->sign ? -rat_ptr->num : rat_ptr->num, rat_ptr->denom);
}

char const* rat_normalize_print(size_t len, char tmp[len], rat const* x)
{
    rat y = rat_copy(x);
    rat_normalize(&y);
    return rat_print(len, tmp, &y);
}

rat* rat_dotproduct(rat rp[static 1], size_t n, rat const A[n], rat const B[n])
{
    rp[0] = rat_get_prod(A[0], B[0]);

    for (size_t i = 1; i < n; ++i)
    {
        rp = rat_rma(rp, A[i], B[i]);
    }

    return rp;
}

int main(void)
{
    rat r = rat_get(24, 57);

    const size_t len = 256;
    char temp[len];
    rat_print(len, temp, &r);
    printf("%s\n", temp);

    rat_normalize_print(len, temp, &r);
    printf("%s\n", temp);

    return EXIT_SUCCESS;
}
