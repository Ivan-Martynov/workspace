#include <codecvt>
#include <filesystem>
#include <iostream>
#include <string>

static void display_file_names(const std::filesystem::path& dir_path,
    const bool recursive = false, size_t depth = 0)
{
    for (size_t i {0}; i < depth; ++i)
    {
        std::cout << " ";
    }
    std::cout << "┌" << dir_path << "\n";
    ++depth;

    for (const std::filesystem::directory_entry& item :
        std::filesystem::directory_iterator {dir_path})
    {
        if (item.is_directory())
        {
            if (recursive)
            {
                display_file_names(item.path(), recursive, depth);
            }
        }
        else
        {
            for (size_t i {0}; i < depth; ++i)
            {
                std::cout << " ";
            }
            std::cout << "├" << item.path() << "\n";
        }
    }
}

static void process_directory(const std::string& directory_path)
{
    auto p {std::filesystem::path {directory_path}};
    if (!std::filesystem::exists(p))
    {
        std::cerr << "Given path " << directory_path << " doesn't exist\n";
    }
    else
    {
        p.make_preferred();
        display_file_names(p, true);
    }
}

int main(const int argc, const char* argv[])
{
    std::setlocale(LC_ALL, " ");

    if (argc > 1)
    {
        process_directory(argv[1]);
    }
    else
    {
        process_directory(".");
    }

    return 0;
}
