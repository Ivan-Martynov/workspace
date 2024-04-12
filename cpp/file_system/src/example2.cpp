#include <cstring>
#include <filesystem>
#include <iostream>
#include <string>

union mbstate_t_ {
  unsigned char bytes[sizeof(mbstate_t)];
  mbstate_t state;
} mbstate_t_;

static size_t multibyte_string_length(const char src[1])
{
    std::mbstate_t state {};
    std::size_t len {std::mbsrtowcs(nullptr, &src, 0, &state)};

    // Restore errno on error.
    if (len == static_cast<std::size_t>(-1))
    {
        errno = 0;
    }

    return len;
}

static size_t multibyte_string_length(
    const std::string& src, const mbstate_t* state_ptr)
{
    if (!state_ptr)
    {
        return multibyte_string_length(src.data());
        //union mbstate_t_ t {};
        //state_ptr = &t.state;
    }

    mbstate_t state = *state_ptr;
    const char* src_data {src.data()};
    size_t len = std::mbsrtowcs(nullptr, &src_data, 0, &state);

    if (len == static_cast<size_t>(-1))
    {
        errno = 0;
    }

    return len;
}

static void test_size_multibyte_string(const std::string& src)
{
    const size_t n {src.size()};
#if 0
    const size_t len {std::mbstowcs(nullptr, src.c_str(), n)};
#else
    size_t len {multibyte_string_length(src, nullptr)};
#endif

    std::cout << "Size of " << std::quoted(src) << ": " << n << ", " << len
              << '\n';
    std::cout << std::hex << std::uppercase << std::setfill('0') << "[";
    for (const unsigned char c : src)
    {
        std::cout << c;
        if (c & 0x80)
        {
            std::cout << "(Mb ";
        }
        else
        {
            std::cout << "(B ";
        }
        std::cout << "0x" << std::setw(2) << static_cast<int>(c) << "),";
    }
    std::cout << std::dec << "]\n";
}

static void display_file_names(const std::filesystem::path& dir_path,
    const bool recursive = false, size_t depth = 0)
{
#if 0
    for (size_t i {0}; i < depth; ++i)
    {
        std::cout << " ";
    }
    std::cout << "┌" << dir_path << "\n";
    ++depth;
#else
    test_size_multibyte_string(dir_path.string());
#endif

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
#if 0
            for (size_t i {0}; i < depth; ++i)
            {
                std::cout << " ";
            }
            std::cout << "├" << item.path() << "\n";
#else
            test_size_multibyte_string(item.path().string());
#endif
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

int main(const int argc, const char* argv[])
{
    std::setlocale(LC_ALL, "");

    std::string item_path {argc > 1 ? argv[1] : "."};

    process_directory(item_path);
    //test_size_multibyte_string(item_path);

    return 0;
}
