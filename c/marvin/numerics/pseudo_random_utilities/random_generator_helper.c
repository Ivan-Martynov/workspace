#include "random_generator_helper.h"
#include "../../type_utilities/type_constants.h"

#include <stdlib.h>
#include <time.h>

/**
 * @brief Seeding pseudo-random generator using srand
 *
 * @param seed The seed value.
 *
 * @version 0.1
 *
 * @date 2023-09-06
 */
void mrvn_seed_simple_generator(const unsigned int seed)
{
    srand(seed);
}

/**
 * @brief Seeding pseudo-random generator using srand with time and null
 * pointer.
 *
 * @version 0.1
 *
 * @date 2023-09-06
 */
void mrvn_seed_simple_generator_default()
{
    mrvn_seed_simple_generator(time(mrvn_null_ptr));
}

/**
 * @brief Implementation part for calculation of a size_t random value in range
 * [@lower_limit, @upper_limit).
 *
 * @param lower_limit Lower limit inclusive.
 * @param upper_limit Upper limit exclusive.
 *
 * @return size_t Pseudo-random value in range [@lower_limit, @upper_limit).
 *
 * @version 0.1
 *
 * @date 2023-09-06
 */
static size_t mrvn_simple_rand_size_t_between_impl(
    const size_t lower_limit, const size_t upper_limit)
{
    return lower_limit + (size_t)rand() % (upper_limit - lower_limit);
}

/**
 * @brief Calculate a size_t random value in range [@lower_limit, @upper_limit).
 *
 * @param lower_limit Lower limit inclusive.
 * @param upper_limit Upper limit exclusive.
 *
 * @return size_t Pseudo-random value in range [@lower_limit, @upper_limit).
 *
 * @version 0.1
 * 
 * @date 2023-09-06
 */
size_t mrvn_simple_rand_size_t_between(
    const size_t lower_limit, const size_t upper_limit)
{
    return (upper_limit > lower_limit)
               ? mrvn_simple_rand_size_t_between_impl(lower_limit, upper_limit)
               : (lower_limit > upper_limit)
                     ? mrvn_simple_rand_size_t_between_impl(
                         upper_limit, lower_limit)
                     : lower_limit;
}

/**
 *  @brief Calculate a size_t random value in range [0, @upper_limit).
 *
 * @param upper_limit Upper limit exclusive.
 *
 * @return size_t Pseudo-random value in range [0, @upper_limit).
 * 
 * @version 0.1
 *
 * @date 2023-09-06
 */
size_t mrvn_simple_rand_size_t_below(const size_t upper_limit)
{
    return mrvn_simple_rand_size_t_between(0U, upper_limit);
}
