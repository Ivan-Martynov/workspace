#include <stdio.h>
#include <stdbool.h>

bool no_more_coins(double* const coin_values)
{
    return coin_values == NULL;
}

size_t count_change(
    const double amount, const size_t coin_count, const double* coin_values)
{
    if (amount == 0)
    {
        return 1;
    }
    else if ((amount < 0) || (coin_count == 0))
    {
        return 0;
    }
    else
    {
        return count_change(amount, coin_count - 1, coin_values + 1)
               + count_change(amount - coin_values[0], coin_count, coin_values);
    }
}

int main()
{
    const double us_coins[] = {50, 25, 10, 5, 1};
    const double uk_coins[] = {100, 50, 20, 10, 5, 2, 1, 0.5};

    const size_t amount = 100;

    const size_t us_count
        = count_change(amount, sizeof(us_coins) / sizeof(double), us_coins);
    printf("Change %zu with US coins = %zu\n", amount,
        count_change(amount, sizeof(us_coins) / sizeof(double), us_coins));

    const size_t uk_count
        = count_change(amount, sizeof(uk_coins) / sizeof(double), uk_coins);
    printf("Change %zu with UK coins = %zu\n", amount,
        count_change(amount, sizeof(uk_coins) / sizeof(double), uk_coins));

    return 0;
}
