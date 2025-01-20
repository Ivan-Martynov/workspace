#ifndef H_INCLUDE_ALGORITHMS_SORTING_H_H
#define H_INCLUDE_ALGORITHMS_SORTING_H_H

#include <iostream>
#include <iterator>
#include <span>
#include <vector>

namespace Marvin
{

template <class E>
concept default_erasable = requires(E* p) { std::destroy_at(p); };

template <class E, class T, class A>
concept allocator_erasable = requires(A m, E* p) {
    requires std::same_as<typename T::allocator_type,
        typename std::allocator_traits<A>::rebind_alloc<E>>;
    std::allocator_traits<A>::destroy(m, p);
};

template <class T>
concept allocator_aware = requires(T a) {
    { a.get_allocator() } -> std::same_as<typename T::allocator_type>;
};

template <class T>
struct is_basic_string : std::false_type {
};

template <class C, class T, class A>
struct is_basic_string<std::basic_string<C, T, A>> : std::true_type {
};

template <class T>
constexpr bool is_basic_string_v = is_basic_string<T>::value;

template <class E, class T>
concept erasable = (is_basic_string_v<T> && default_erasable<E>)
                   || (allocator_aware<T>
                       && allocator_erasable<E, T, typename T::allocator_type>)
                   || (!allocator_aware<T> && default_erasable<E>);

template <class T>
concept container = requires(T a, const T b) {
    requires std::regular<T>;
    requires std::swappable<T>;
    requires erasable<typename T::value_type, T>;
    requires std::same_as<typename T::reference, typename T::value_type&>;
    requires std::same_as<typename T::const_reference,
        const typename T::value_type&>;
    requires std::forward_iterator<typename T::iterator>;
    requires std::forward_iterator<typename T::const_iterator>;
    requires std::signed_integral<typename T::difference_type>;
    requires std::same_as<typename T::difference_type,
        typename std::iterator_traits<typename T::iterator>::difference_type>;
    requires std::same_as<typename T::difference_type,
        typename std::iterator_traits<
            typename T::const_iterator>::difference_type>;
    { a.begin() } -> std::same_as<typename T::iterator>;
    { a.end() } -> std::same_as<typename T::iterator>;
    { b.begin() } -> std::same_as<typename T::const_iterator>;
    { b.end() } -> std::same_as<typename T::const_iterator>;
    { a.cbegin() } -> std::same_as<typename T::const_iterator>;
    { a.cend() } -> std::same_as<typename T::const_iterator>;
    { a.size() } -> std::same_as<typename T::size_type>;
    { a.max_size() } -> std::same_as<typename T::size_type>;
    { a.empty() } -> std::convertible_to<bool>;
};

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

template <typename Comp>
bool is_sorted(container auto array, Comp compare_function)
{
    return is_sorted(array.begin(), array.end(), compare_function);
}

bool is_sorted(container auto array)
{
    return is_sorted(array.begin(), array.end());
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

template <typename Comp>
void merge_sort(std::random_access_iterator auto first,
    std::random_access_iterator auto last, Comp compare_function)
{
    // Placing the merge function inside as a lambda for encapsulation.
    auto merge_parts_lambda {[&compare_function](
                                 std::random_access_iterator auto left,
                                 std::random_access_iterator auto middle,
                                 std::random_access_iterator auto right) {
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
    }};

    if (const auto diff {last - first}; diff > 0) {
        const auto center {first + (diff >> 1)};
        merge_sort(first, center, compare_function);
        merge_sort(center + 1, last, compare_function);
        merge_parts_lambda(first, center, last);
    }
}

void merge_sort(std::random_access_iterator auto first,
    std::random_access_iterator auto last)
{
    merge_sort(first, last, std::less {});
}

template <typename Comp>
void merge_sort(container auto array, Comp compare_function)
{
    merge_sort(array.begin(), array.end(), compare_function);
}

void merge_sort(container auto array)
{
    merge_sort(array.begin(), array.end());
}

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
