#include "MP3BatchFileRenamer.h"
#include "MP3Tag.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <ranges>
#include <filesystem>

[[maybe_unused]]
static void process_entry(const std::filesystem::directory_entry& entry,
    const std::vector<std::string_view>& options)
{
    Marvin::MP3Tag mp3_tag {entry, options};
    mp3_tag.show_frames();
}

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
    Marvin::MP3BatchFileRenamer tool {paths, options};
    tool.run();

    return 0;
}