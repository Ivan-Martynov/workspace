#include <filesystem>
#include <iostream>
#include <cstring>
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
    const auto p {std::filesystem::path {directory_path}.make_preferred()};
    if (!std::filesystem::exists(p))
    {
        std::cerr << "Given path " << directory_path << " doesn't exist\n";
    }
    else
    {
        display_file_names(p, true);
    }
}

static void test_size_multibyte_string(const std::string& src)
{
    const size_t n {src.size()};
    const size_t len {std::mbstowcs(nullptr, src.c_str(), n)};
    std::cout << "Size of src: " << src << ": " << n << ", " << len << ", "
              << src.length() << '\n';
    std::cout << std::hex << std::uppercase;
    for (const unsigned char c : src)
    {
        if (c & 0x80)
        {
            std::cout << "Multibyte => ";
        }
        else
        {
            std::cout << "Byte => ";
        }
        std::cout << c << '(' << static_cast<int>(c) << ") ";
    }
    std::cout << std::dec << '\n';
}

int main(const int argc, const char* argv[])
{
    std::setlocale(LC_ALL, "");

    std::string item_path {argc > 1 ? argv[1] : "."};

    process_directory(item_path);
    test_size_multibyte_string(item_path);

    return 0;
}
