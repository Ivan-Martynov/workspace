#ifndef H_NUMBERS_INCLUDE_CONSTANTS_H_H
#define H_NUMBERS_INCLUDE_CONSTANTS_H_H

#include "Rational.h"

namespace Marvin
{

namespace Constants
{

inline constexpr auto one_half {Rational {1, 2}};
inline constexpr auto three_quarters {Rational {3, 4}};

// inline constexpr Rational infinity {Rational {1, 0}};
// inline constexpr Rational neg_infinity {Rational {-1, 0}};
// inline constexpr Rational nan {Rational {0, 0}};

} // namespace Constants

} // namespace Marvin

#endif // H_NUMBERS_INCLUDE_CONSTANTS_H_H
