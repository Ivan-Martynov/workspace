#ifndef _H_RANDOM_GENERATOR_HELPER_H_
#define _H_RANDOM_GENERATOR_HELPER_H_

#include <stddef.h>

/**
 * @brief Seeding pseudo-random generator using srand
 *
 * @param seed The seed value.
 *
 * @version 0.1
 *
 * @date 2023-09-06
 */
void mrvn_seed_simple_generator(const unsigned int seed);

/**
 * @brief Seeding pseudo-random generator using srand with time and null
 * pointer.
 *
 * @version 0.1
 *
 * @date 2023-09-06
 */
void mrvn_seed_simple_generator_default();

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
    const size_t lower_limit, const size_t upper_limit);

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
size_t mrvn_simple_rand_size_t_below(const size_t upper_limit);

#endif //_H_RANDOM_GENERATOR_HELPER_H_