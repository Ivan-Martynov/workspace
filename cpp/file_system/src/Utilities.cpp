#include "Utilities.h"

#include <filesystem>

namespace Marvin
{

std::pair<std::vector<std::string_view>, std::vector<std::string_view>>
collect_paths_and_options(const int argc, const char* argv[])
{
    std::vector<std::string_view> paths {};
    std::vector<std::string_view> options {};

    for (int i {1}; i < argc; ++i)
    {
        // If the argument is not a valid path, then treat it as an option.
        const auto argument {argv[i]};
        if (std::filesystem::exists(argument))
        {
            paths.emplace_back(argument);
        }
        else
        {
            options.emplace_back(argument);
        }
    }

    return std::make_pair(paths, options);
}

} // namespace Marvin
