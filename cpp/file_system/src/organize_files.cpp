#include "FileOrganizer.h"
#include "Utilities.h"

#include <filesystem>
#include <vector>
#include <regex>
#include <iostream>

//int main()
int main(const int argc, const char* argv[])
{
    std::setlocale(LC_ALL, "");

#if 1
    auto paths_n_options {Marvin::collect_paths_and_options(argc, argv)};

    // Process all the paths with provided options.
    Marvin::FileOrganizer {paths_n_options.first, paths_n_options.second}.run();
#else
    try {
        const auto r {std::regex {"(\\)"}};
    } catch (const std::regex_error& e) {
        std::cout << e.what() << "\n";
    }
#endif

    return 0;
}