#include "BatchFileRenamer.hpp"

#include <chrono>
#include <filesystem>
#include <functional>
#include <iostream>
#include <vector>

int main(const int argc, const char* argv[])
{
    std::setlocale(LC_ALL, "");

    // Collect paths and options.
    std::vector<std::string_view> options {};
    std::vector<std::string_view> paths {};
    for (int i {1}; i < argc; ++i)
    {
        const auto argument {argv[i]};
        // Options should always have a dash sign in front.
        if ((argument[0] == '-') || !std::filesystem::exists(argument))
        {
            options.emplace_back(argument);
        }
        else
        {
            paths.emplace_back(argument);
        }
    }

    Marvin::BatchFileRenamer tool {paths, options};
    tool.run();

    return 0;
}