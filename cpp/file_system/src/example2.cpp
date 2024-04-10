#include <codecvt>
#include <filesystem>
#include <iostream>
#include <cstring>
#include <string>

static void display_file_names_wide(const std::filesystem::path& dir_path,
    const bool recursive = false, size_t depth = 0)
{
    for (size_t i {0}; i < depth; ++i)
    {
        std::wcout << L" ";
    }
    std::wcout << L"┌" << dir_path << L"\n";
    ++depth;

    for (const std::filesystem::directory_entry& item :
        std::filesystem::directory_iterator {dir_path})
    {
        if (item.is_directory())
        {
            if (recursive)
            {
                display_file_names_wide(item.path(), recursive, depth);
            }
        }
        else
        {
            for (size_t i {0}; i < depth; ++i)
            {
                std::wcout << L" ";
            }
            std::wcout << L"├" << item.path() << L"\n";
        }
    }
}

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

std::wstring str_to_wstr(const std::string& file_path)
{
    const size_t n {file_path.length()};
    std::wstring result {};
    result.reserve(n);
    for (size_t i {0}; i < n; ++i)
    {
        result.push_back(file_path[i] & 0xFF);
    }
    return result;
}

static void test_wide(const std::string& file_path)
{
    for (size_t i {0}; i < file_path.size(); ++i)
    {
        if (file_path[i] & 0x80)
        {
            std::cout << file_path[i] << "\n";
        }
    }

    //size_t len {file_path.size()};
    //std::wstring s(len, L' ');
    //mbstowcs(s.data(), file_path.c_str(), len);
    mbstate_t state {};
    memset(&state, 0, sizeof(state));

    const char* src {file_path.c_str()};
    const size_t len {1 + std::mbsrtowcs(nullptr, &src, 0, &state)};
    wchar_t* target = static_cast<wchar_t*>(std::malloc(len * sizeof(wchar_t)));
    std::mbsrtowcs(&target[0], &src, len, &state);

    // std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter {};
    // s = converter.from_bytes(file_path);

    // std::wstring s {str_to_wstr(file_path)};
    //std::cout << file_path;
    //std::wcout << L" => " << target << L"\n";
    process_directory(file_path);

    std::free(target);
}

int main(const int argc, const char* argv[])
{
    std::setlocale(LC_ALL, "");

    if (argc > 1)
    {
        //process_directory(argv[1]);
        test_wide(argv[1]);
    }
    else
    {
        process_directory(".");
        //test_wide(".");
    }

    return 0;
}
