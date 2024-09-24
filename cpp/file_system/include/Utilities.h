#ifndef H_INCLUDE_UTILITIES_H
#define H_INCLUDE_UTILITIES_H

#include <string_view>
#include <vector>

namespace Marvin
{

std::pair<std::vector<std::string_view>, std::vector<std::string_view>>
collect_paths_and_options(const int argc, const char* argv[]);

} // namespace Marvin

#endif // H_INCLUDE_UTILITIES_H
