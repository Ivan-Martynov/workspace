#ifndef H_INCLUDE_CONTAINERHELPER_H_H
#define H_INCLUDE_CONTAINERHELPER_H_H

#include <iterator>
#include <concepts>

/**
 * @brief Place various structures/tools to use with different containers.
 *
 */
namespace Marvin
{

/*******************************************************************************
 * Container concept section                                                   *
 ******************************************************************************/

template <typename T>
concept DefaultErasable = requires(T* ptr) { std::destroy_at(ptr); };

template <typename T, typename U, typename A>
concept AllocatorErasable = requires(A a, T* ptr) {
    requires std::same_as<typename U::allocator_type,
        typename std::allocator_traits<A>::rebind_alloc<T>>;
    std::allocator_traits<A>::destroy(a, ptr);
};

template <typename T>
concept AllocatorAware = requires(T t) {
    { t.get_allocator() } -> std::same_as<typename T::allocator_type>;
};

template <typename T>
struct IsBasicString : std::false_type {
};

template <typename T, typename U, typename A>
struct IsBasicString<std::basic_string<T, U, A>> : std::true_type {
};

template <typename T>
constexpr bool IsBasicString_v = IsBasicString<T>::value;

template <typename T, typename U>
concept Erasable = (IsBasicString_v<U> && DefaultErasable<T>)
                   || (AllocatorAware<U>
                       && AllocatorErasable<T, U, typename U::allocator_type>)
                   || (!AllocatorAware<U> && DefaultErasable<T>);

template <typename T>
concept Regular = std::semiregular<T> && std::equality_comparable<T>;

template <typename T>
concept Container = requires(T a, const T b) {
    //requires std::reqular<T>;
    requires Regular<T>;
    requires std::swappable<T>;
    requires Erasable<typename T::value_type, T>;
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

/*******************************************************************************
 * End of Container concept section                                            *
 ******************************************************************************/

/**
 * @brief Tag class to use with constructors to distinguish from constructors
 * having initializer_list as a parameter.
 *
 */
class init_container_with_size_t
{
};
constexpr init_container_with_size_t init_container_with_size {};

} // namespace Marvin

#endif // H_INCLUDE_CONTAINERHELPER_H_H
