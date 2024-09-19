#include "MP3Tag.hpp"

#include <iostream>
#include <vector>
#include <algorithm>
#include <ranges>
#include <filesystem>

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

    bool recursive {false};
    for (const auto& option: options)
    {
        if ((option == "-r") || (option == "-recursive"))
        {
            recursive = true;
        }
    }

    if (recursive)
    {
        for (const auto& path : paths)
        {
            for (const std::filesystem::directory_entry& entry :
                std::filesystem::recursive_directory_iterator {path})
            {
                process_entry(entry, options);
            }
        }
    }
    else
    {
        for (const auto& path : paths)
        {
            for (const std::filesystem::directory_entry& entry :
                std::filesystem::directory_iterator {path})
            {
                process_entry(entry, options);
            }
        }
    }

    return 0;
}