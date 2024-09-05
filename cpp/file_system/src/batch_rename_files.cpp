#include "BatchFileRenamer.hpp"
#include "FilenameReplaceDecorator.hpp"
#include "ConcreteFileDecorator.hpp"

#include <chrono>
#include <filesystem>
#include <functional>
#include <iostream>
#include <vector>

static void process_path_items(
    const std::filesystem::path& dir_path, const bool recursive = false)
{
    std::vector<std::filesystem::directory_entry> entries {};
    if (recursive)
    {
        for (const std::filesystem::directory_entry& item :
            std::filesystem::recursive_directory_iterator {dir_path})
        {
            entries.emplace_back(item);
        }
    }
    else
    {
        for (const std::filesystem::directory_entry& item :
            std::filesystem::directory_iterator {dir_path})
        {
            entries.emplace_back(item);
        }
    }

    // Sorting by name.
    std::sort(entries.begin(), entries.end());
#if 0
    // Sort by time.
    std::sort(entries.begin(), entries.end(),
        [](auto a, auto b)
        {
            return a.last_write_time() > b.last_write_time();
        });
#endif

#if 1
    // Sort by size.
    std::sort(entries.begin(), entries.end(),
        [](auto a, auto b)
        {
            if (a.is_directory())
            {
                return b.is_directory() ? a < b : false;
            }
            else if (b.is_directory())
            {
                return true;
            }

            return a.file_size() < b.file_size();
        });
#endif

    //for (auto& entry : entries)
    //{
    //    std::wcout << L"Entry: " << entry << L"\n";
    //}
}

static void show_files_in_directory(
    const std::string& target_path_name, const bool recursive = false)
{
    const auto target_path {
        std::filesystem::path {target_path_name}.make_preferred()};
    if (!std::filesystem::exists(target_path))
    {
        std::cerr << "Given path " << target_path_name << " doesn't exist\n";
    }
    else
    {
        process_path_items(target_path, recursive);
        //process_path_items(target_path, print_last_write_time, recursive);
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
        if ((argument[0] == '-') || !std::filesystem::exists(argument))
        {
            options.emplace_back(argument);
        }
        else
        {
            paths.emplace_back(argument);
        }
    }

    std::wstring match {L""};
    std::wstring replacement {L""};

    bool recursive {false};
    bool verbose {false};

    for (size_t i {0}; i < options.size(); ++i)
    {
        const auto option {options[i]};
        if ((option == "-rur") || (option == "-replace-using-regex"))
        {
            if (++i < options.size())
            {
                // A trick to convert string to wstring.
                match = std::filesystem::path(options[i]).wstring();
            }

            if (++i < options.size())
            {
                replacement = std::filesystem::path(options[i]).wstring();
            }
        }
        else if ((option == "-r") || (option == "-recursive"))
        {
            recursive = true;
        }
        else if ((option == "-v") || (option == "-verbose"))
        {
            verbose = true;
        }
    }

    Marvin::BatchFileRenamer tool {paths, recursive, verbose};
    //tool.replace_spaces_with_underscores();
    if (!match.empty())
    {
        tool.replace_using_regex(match, replacement);
    }

    for (const std::string& target_path_name : paths)
    {
        auto decorator_ptr {
            std::make_unique<Marvin::ConcreteFileDecorator>(target_path_name)};
        auto replace_decorator_ptr {
            std::make_unique<Marvin::FilenameReplaceDecorator>(
                decorator_ptr.get())};

        decorator_ptr->execute();
        replace_decorator_ptr->execute();

        show_files_in_directory(target_path_name, recursive);
    }

    return 0;
}