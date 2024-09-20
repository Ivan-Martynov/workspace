#include "FileOrganizer.h"

#include <filesystem>
#include <vector>

int main(const int argc, const char* argv[])
{
    std::setlocale(LC_ALL, "");

    // Collect paths and options.
    std::vector<std::string_view> options {};
    std::vector<std::string_view> paths {};
    for (int i {1}; i < argc; ++i)
    {
        // If the argument is not a valid path, then treat it as an option.
        const auto argument {argv[i]};
        if (std::filesystem::exists(argument)
            && std::filesystem::is_directory(argument))
        {
            paths.emplace_back(argument);
        }
        else
        {
            options.emplace_back(argument);
        }
    }

    // Process all the paths with provided options.
    Marvin::FileOrganizer tool {paths, options};
    tool.run();

    return 0;
}