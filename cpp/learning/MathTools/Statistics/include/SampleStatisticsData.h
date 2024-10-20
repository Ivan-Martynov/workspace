#ifndef H_STATISTICS_INCLUDE_SAMPLESTATISTICSDATA_H_H
#define H_STATISTICS_INCLUDE_SAMPLESTATISTICSDATA_H_H

#include <vector>
#include <numeric>
#include <optional>
#include <functional>

namespace Marvin
{

template <typename T>
class SampleStatisticsData
{
  private:
    // Define zero value for the given arithmetic type.
    static constexpr T zero_value {static_cast<T>(0)};

    enum class Features
    {
        sum,
        mean,
    };

    // Data representing sample.
    std::vector<T> m_data;

    std::optional<T> m_sum {};
    std::optional<T> m_mean {};

    auto m_get_value_lambda(auto&& target, auto&& fun)
    {
        if (!target.has_value())
        {
            target = fun();
        }
        return target.value();
    };

    auto m_calculate_sum()
    {
        return std::accumulate(m_data.cbegin(), m_data.cend(), zero_value);
    }

    auto m_calculate_mean()
    {
        const auto n {m_data.size()};
        return n ? sum() / n : zero_value;
    }

  public:
    explicit SampleStatisticsData<T>(const std::vector<T>& sample)
        : m_data {sample}
    {
    }

    /**
     * @brief Sum of sample data.
     *
     * @return T Sum.
     */
    T sum()
    {
        return m_get_value_lambda(
            m_sum, std::bind(&SampleStatisticsData::m_calculate_sum, this));
    }

    /**
     * @brief Mean value of sample.
     *
     * @return T Mean.
     */
    T mean()
    {
        return m_get_value_lambda(
            m_mean, std::bind(&SampleStatisticsData::m_calculate_mean, this));
    }
};

} // namespace Marvin

#endif // H_STATISTICS_INCLUDE_SAMPLESTATISTICSDATA_H_H
