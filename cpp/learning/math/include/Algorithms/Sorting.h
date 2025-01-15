#ifndef H_INCLUDE_ALGORITHMS_SORTING_H_H
#define H_INCLUDE_ALGORITHMS_SORTING_H_H

#include <iostream>
#include <iterator>
#include <span>
#include <vector>

namespace Marvin
{

template <typename Comp>
bool is_sorted(std::random_access_iterator auto first,
    std::random_access_iterator auto last, Comp compare_function)
{
    while (++first < last) {
        if (compare_function(first, first - 1)) {
            return false;
        }
    }
    return true;
}

bool is_sorted(std::random_access_iterator auto first,
    std::random_access_iterator auto last)
{
    return is_sorted(first, last, std::less {});
}

/**
 * @brief Sort a range, represented by two random access iterators, using a
 * comparison function.
 *
 * @tparam Comp template parameter for the comparsion function.
 * @param[in] first Start of the range.
 * @param[in] last End of the range.
 * @param[in] compare_function Comparison function to compare two elements.
 */
template <typename Comp>
void insert_sort(std::random_access_iterator auto first,
    std::random_access_iterator auto last, Comp compare_function)
{
    /**
     * @brief Analogy is sorting cards in a hand. Whenever a new card is added
     * (inserted), the appropriate place is search for. Here the items ('cards')
     * are already in the 'hand', so the range is sorted in place. Starting from
     * the second, each item is compared to previous items, which are already
     * sorted, and placed in the correct place, shifting other elements
     * accordingly.
     */
    // No need to start from the first element - it is already 'sorted'.
    for (auto it {first + 1}; it < last; ++it) {
        const auto key {*it};
        auto prev {it};
        // Scan through the preceding items to place so that the elements are
        // sorted.
        while ((prev-- != first) && (compare_function(key, *prev))) {
            *(prev + 1) = *prev;
        }
        // Place the element into the correct position.
        *(++prev) = key;
    }
}

void insert_sort(std::random_access_iterator auto first,
    std::random_access_iterator auto last)
{
    insert_sort(first, last, std::less {});
}

template <typename T, typename Comp>
void insert_sort(std::span<T> range, Comp compare_function)
{
    insert_sort(range.begin(), range.end(), compare_function);
}

template <typename T>
void insert_sort(std::span<T> range)
{
    insert_sort(range, std::less {});
}

template <typename Comp>
void selection_sort(std::random_access_iterator auto first,
    std::random_access_iterator auto last, Comp compare_function)
{
    if (first == last) {
        return;
    }

    for (auto it {first}; it < last - 1; ++it) {
        auto min_it {it};
        for (auto next {it + 1}; next < last; ++next) {
            if (compare_function(*next, *min_it)) {
                min_it = next;
            }
        }
        using std::swap;
        swap(*it, *min_it);
    }
}

void selection_sort(std::random_access_iterator auto first,
    std::random_access_iterator auto last)
{
    selection_sort(first, last, std::less {});
}

template <typename Comp>
void bubble_sort(std::random_access_iterator auto first,
    std::random_access_iterator auto last, Comp compare_function)
{
    if (first == last--) {
        return;
    }

    while (true) {
        bool no_change {true};
        for (auto it {first}; it < last; ++it) {
            if (compare_function(*(it + 1), *it)) {
                using std::swap;
                swap(*(it + 1), *it);
                no_change = false;
            }
        }

        // No swap has been performed - meaning the array is sorted.
        if (no_change) {
            break;
        }

        // The last element is anyway at its correct position.
        --last;
    }
}

void bubble_sort(std::random_access_iterator auto first,
    std::random_access_iterator auto last)
{
    bubble_sort(first, last, std::less {});
}

#if 1
template <typename Comp>
void merge_parts(std::random_access_iterator auto left,
    std::random_access_iterator auto middle,
    std::random_access_iterator auto right, Comp compare_function)
{
    const auto next {std::next(middle)};
    std::vector left_block(left, next);
    std::vector right_block(next, std::next(right));
    const auto n1 {static_cast<int>(left_block.size())};
    const auto n2 {static_cast<int>(right_block.size())};

    int i {0};
    int j {0};
    int k {0};
    while ((i < n1) && (j < n2)) {
        *(left + k++) = (compare_function(left_block[i], right_block[j]))
                            ? left_block[i++]
                            : right_block[j++];
    }
    while (i < n1) {
        *(left + k++) = left_block[i++];
    }
    while (j < n2) {
        *(left + k++) = right_block[j++];
    }
}

template <typename Comp>
void merge_sort(std::random_access_iterator auto left,
    std::random_access_iterator auto right, Comp compare_function)
{
    if (const auto diff {right - left}; diff > 0) {
        const auto middle {left + (diff >> 1)};
        merge_sort(left, middle, compare_function);
        merge_sort(middle + 1, right, compare_function);
        merge_parts(left, middle, right, compare_function);
    }
}

void merge_sort(std::random_access_iterator auto first,
    std::random_access_iterator auto last)
{
    merge_sort(first, last, std::less {});
}

#endif

#if 0
class ISortingAlgorithm
{
  public:
    void sort(std::random_access_iterator auto first,
        std::random_access_iterator auto last);

    virtual ~ISortingAlgorithm() = default;
};
#endif

} // namespace Marvin

#endif // H_INCLUDE_ALGORITHMS_SORTING_H_H
