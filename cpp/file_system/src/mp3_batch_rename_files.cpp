#include "MP3BatchFileRenamer.h"
#include "MP3Tag.h"

#include "Utilities.h"

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

    auto paths_n_options {Marvin::collect_paths_and_options(argc, argv)};

    // Process all the paths with provided options.
    Marvin::MP3BatchFileRenamer {paths_n_options.first, paths_n_options.second}
        .run();

    return 0;
}