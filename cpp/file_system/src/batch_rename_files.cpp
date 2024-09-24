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
#include "Utilities.h"

#include <filesystem>
#include <vector>

int main(const int argc, const char* argv[])
{
    std::setlocale(LC_ALL, "");

    auto paths_n_options {Marvin::collect_paths_and_options(argc, argv)};

    // Process all the paths with provided options.
    Marvin::BatchFileRenamer {paths_n_options.first, paths_n_options.second}
        .run();

    return 0;
}