#ifndef H_INCLUDE_CONTAINERHELPER_H_H
#define H_INCLUDE_CONTAINERHELPER_H_H

/**
 * @brief Place various structures/tools to use with different containers.
 *
 */
namespace Marvin
{

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
