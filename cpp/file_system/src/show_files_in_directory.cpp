#include <iostream>
#include <vector>
#include <filesystem>

/**
 * @brief Display names of items in the given directory.
 * 
 * @param dir_path Path to the directory/folder.
 * @param recursive Flag whether inner directories should be scanned too.
 * 
 * @version 0.1
 * 
 * @date 2024-06-27
 */
static void display_file_names(const std::filesystem::path& dir_path,
    const bool recursive = false)
{
    // Loop through items in the given directory.
    for (const std::filesystem::directory_entry& item :
        std::filesystem::directory_iterator {dir_path})
    {
        // Displaying items.
        if (item.is_directory())
        {
            std::cout << "Directory: " << item.path() << "\n";

            // If scanning recursively, then process the directory item.
            if (recursive)
            {
                display_file_names(item.path(), recursive);
            }
        }
        else
        {
            std::cout << "File: " << item.path() << "\n";
        }
    }
}

static void show_files_in_directory(const std::string& directory_path,
    const bool recursive = false)
{
    const auto p {std::filesystem::path {directory_path}.make_preferred()};
    if (!std::filesystem::exists(p))
    {
        std::cerr << "Given path " << directory_path << " doesn't exist\n";
    }
    else
    {
        display_file_names(p, recursive);
    }
}

int main(const int argc, const char* argv[])
{
    std::setlocale(LC_ALL, "");

    // Collect paths and options.
    std::vector<std::string> options {};
    std::vector<std::string> paths {};
    for (int i {1}; i < argc; ++i)
    {
        const auto argument {argv[i]};
        // Options should always have a dash sign in front.
        if (argument[0] == '-')
        {
            options.emplace_back(argument);
        }
        else
        {
            paths.emplace_back(argument);
        }
    }

    bool recursive {false};
    for (std::string &option : options)
    {
        // Flag for recursion.
        if (option == "-r")
        {
            recursive = true;
        }
    }

    for (const std::string& p : paths)
    {
        show_files_in_directory(p, recursive);
    }
 
    return 0;
}