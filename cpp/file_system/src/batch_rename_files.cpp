/**
 * @file batch_rename_files.cpp
 * @brief Simple program to rename all files (and/or directories) in the paths
 * provided as arguments to the program. Other arguments define options for file
 * names modifications.
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "BatchFileRenamer.h"

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
        const std::string_view argument {argv[i]};
        if (std::filesystem::exists(argument))
        {
            paths.emplace_back(argument);
        }
        else
        {
            options.emplace_back(argument);
        }
    }

    // Process all the paths with provided options.
    Marvin::BatchFileRenamer tool {paths, options};
    tool.run();

    return 0;
}