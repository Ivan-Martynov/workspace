#ifndef _H_FILE_RENAME_STRATEGY_H_
#define _H_FILE_RENAME_STRATEGY_H_

#include <algorithm>
#include <string>
#include <vector>
#include <iostream>

namespace Marvin
{

class FileRenameStrategy
{
  protected:
    std::vector<std::string> m_options;

    enum class Targets
    {
        NONE = 0,
        FILES = 1,
        DIRECTORIES = 2,
        EXTENSIONS = 4,
    };

    enum class Sorting
    {
        NONE = 0,
        BY_NAMES = 1,
        BY_SIZE = 2,
        BY_TIMESTAMP = 4,
        DESCENDING = 8,
    };

    bool m_recursive {false};
    bool m_verbose {false};
    bool do_modify {false};

    size_t m_targets {static_cast<size_t>(Targets::FILES)};
    size_t m_sorting {static_cast<size_t>(Sorting::NONE)};

  public:
    explicit FileRenameStrategy(const std::vector<std::string>& options)
        : m_options {options}
    {
        for (const auto& option : m_options)
        {
            if ((option == "-r") || (option == "-recursive"))
            {
                m_recursive = true;
            }
            else if ((option == "-v") || (option == "-verbose"))
            {
                m_verbose = true;
            }
            else if ((option == "-dm") || (option == "-do-modify"))
            {
                do_modify = true;
            }
            else if ((option == "-nf") || (option == "-no-filenames"))
            {
                m_targets = static_cast<size_t>(Targets::NONE);
            }
            else if ((option == "-d") || (option == "-directories"))
            {
                m_targets |= static_cast<size_t>(Targets::DIRECTORIES);
            }
            else if ((option == "-do") || (option == "-directories-only"))
            {
                m_targets = static_cast<size_t>(Targets::DIRECTORIES);
            }
            else if ((option == "-e") || (option == "-extensions"))
            {
                m_targets |= static_cast<size_t>(Targets::EXTENSIONS);
            }
            else if ((option == "-e") || (option == "-extensions"))
            {
                m_targets = static_cast<size_t>(Targets::EXTENSIONS);
            }
            else if ((option == "-sbn") || (option == "-sort-by-name"))
            {
                m_sorting |= static_cast<size_t>(Sorting::BY_NAMES);
            }
            else if ((option == "-sbs") || (option == "-sort-by-size"))
            {
                std::wcout << L"Sorting byt size?\n";
                m_sorting |= static_cast<size_t>(Sorting::BY_SIZE);
            }
            else if ((option == "-sbt") || (option == "-sort-by-timestamp"))
            {
                m_sorting |= static_cast<size_t>(Sorting::BY_TIMESTAMP);
            }
            else if ((option == "-sd") || (option == "-sort-descending"))
            {
                m_sorting |= static_cast<size_t>(Sorting::DESCENDING);
            }
        }
    }

    virtual ~FileRenameStrategy() = default;

    //virtual void modify_filepath(std::filesystem::path&) = 0;
    virtual void modify_entry(std::filesystem::directory_entry&) = 0;

    virtual void modify_files_in_dir(const std::string& dir_path)
    {
        std::vector<std::filesystem::directory_entry> files {};
        std::vector<std::filesystem::directory_entry> directories {};

        for (const std::filesystem::directory_entry& item :
            std::filesystem::directory_iterator {dir_path})
        {
            if (item.is_directory())
            {
                if (m_recursive)
                {
                    modify_files_in_dir(item.path());
                }

                if (m_targets & static_cast<size_t>(Targets::DIRECTORIES))
                {
                    directories.emplace_back(item);
                }
            }
            else if (item.is_regular_file())
            {
                files.emplace_back(item);
            }
        }

        const bool ascending {
            (m_sorting & static_cast<size_t>(Sorting::DESCENDING)) == 0};

        // Sort by names to have unique sorting if the flags is set.
        if (m_sorting & static_cast<size_t>(Sorting::BY_NAMES))
        {
            std::wcout << L"Sorting by size\n";
            if (ascending)
            {
                std::sort(files.begin(), files.end());
                std::sort(directories.begin(), directories.end());
            }
            else
            {
                std::sort(files.begin(), files.end(), std::greater<>());
                std::sort(
                    directories.begin(), directories.end(), std::greater<>());
            }
        }

        // Sort by file size to have unique sorting if the flags is set. Using
        // stable sort to preserve order. Directories are not sorted by size.
        if (m_sorting & static_cast<size_t>(Sorting::BY_SIZE))
        {
            if (ascending)
            {
                std::sort(files.begin(), files.end(), [](auto& a, auto& b)
                    { return a.file_size() < b.file_size(); });
            }
            else
            {
                std::sort(files.begin(), files.end(), [](auto& a, auto& b)
                    { return a.file_size() > b.file_size(); });
            }
        }

        // Sort by timestamp to have unique sorting if the flags is set. Using
        // stable sort to preserve order.
        if (m_sorting & static_cast<size_t>(Sorting::BY_TIMESTAMP))
        {
            if (ascending)
            {
                auto f_ascending {[](auto& a, auto& b)
                    { return a.last_write_time() < b.last_write_time(); }};

                std::stable_sort(files.begin(), files.end(), f_ascending);
                std::stable_sort(
                    directories.begin(), directories.end(), f_ascending);
            }
            else
            {
                auto f_descending {[](auto& a, auto& b)
                    { return a.last_write_time() > b.last_write_time(); }};

                std::stable_sort(files.begin(), files.end(), f_descending);
                std::stable_sort(
                    directories.begin(), directories.end(), f_descending);
            }
        }

        for (std::filesystem::directory_entry& entry : files)
        {
            //auto p = entry.path();
            //modify_filepath(p);
            modify_entry(entry);
        }

        for (std::filesystem::directory_entry& entry : directories)
        {
            //auto p {entry.path()};
            //modify_filepath(p);
            modify_entry(entry);
        }
    }
};

} // namespace Marvin

#endif // _H_FILE_RENAME_STRATEGY_H_
