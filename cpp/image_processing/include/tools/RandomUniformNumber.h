#ifndef H_INCLUDE_TOOLS_RANDOMUNIFORMINT_H_H
#define H_INCLUDE_TOOLS_RANDOMUNIFORMINT_H_H

#include <chrono>
#include <random>

namespace Marvin
{

template <typename T>
concept Arithmetic = std::integral<T> || std::floating_point<T>;

/**
 * @brief Static class providing access to random device, seed and engine.
 *
 */
class RandomUniformEngine
{
  public:
    RandomUniformEngine() = delete;

    static std::random_device random_device;
    static std::seed_seq seed;
    static std::mt19937 mersenne_twister_engine;
};

std::random_device RandomUniformEngine::random_device {};

std::seed_seq RandomUniformEngine::seed {
    static_cast<std::seed_seq::result_type>(
        std::chrono::steady_clock::now().time_since_epoch().count()),
    random_device(), random_device(), random_device(), random_device(),
    random_device(), random_device(), random_device()};

std::mt19937 RandomUniformEngine::mersenne_twister_engine {seed};

template <Arithmetic T = int>
class RandomUniformNumber
{
  public:
    using value_type = T;

    RandomUniformNumber(Arithmetic auto a, Arithmetic auto b) : m_dist {a, b} {}

    RandomUniformNumber(Arithmetic auto upper)
        : RandomUniformNumber {static_cast<value_type>(0), upper}
    {
    }

    RandomUniformNumber() : RandomUniformNumber {m_default_upper()} {}

    value_type generate()
    {
        return m_dist(RandomUniformEngine::mersenne_twister_engine);
    }

  private:
    std::conditional_t<std::is_integral_v<value_type>,
        std::uniform_int_distribution<value_type>,
        std::uniform_real_distribution<value_type>>
        m_dist;

    static constexpr value_type m_default_upper()
    {
        if constexpr (std::is_integral_v<value_type>) {
            return std::numeric_limits<value_type>::max();
        } else {
            return 1.0;
        }
    }
};

} // namespace Marvin

#endif // H_INCLUDE_TOOLS_RANDOMUNIFORMINT_H_H
