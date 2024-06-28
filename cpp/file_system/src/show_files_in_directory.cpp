#include <chrono>
#include <filesystem>
#include <functional>
#include <iostream>
#include <vector>

static std::string locale_name {};

static std::wstring string_to_wstring(const std::string& src)
{
    std::wstring result(src.length() + 1, L'\0');
    std::mbstowcs(result.data(), src.c_str(), result.length());

    return result;
}

static std::time_t get_last_modification_time(
    const std::filesystem::path& target_path)
{
    const auto last_modification_time {
        std::filesystem::last_write_time(target_path)};

    return std::chrono::system_clock::to_time_t(
        std::chrono::file_clock::to_sys(last_modification_time));
}

[[maybe_unused]]
static void print_last_write_time(const std::filesystem::path& target_path)
{
    auto loc {std::locale()};
    try
    {
        loc = std::locale(locale_name);
        if (!std::setlocale(LC_TIME, locale_name.c_str()))
        {
            std::wcout << L"Failed to set locale " << locale_name.c_str()
                << '\n';
        }
        else
        {
            std::locale::global(loc);
        }
    }
    catch (const std::exception& e)
    {
        std::wcerr << L"Failed to set locale " << locale_name.c_str() << ": "
            << e.what() << "\n";
        loc = std::locale();
    }

    // const auto current_locale {setlocale(LC_TIME, nullptr)};
    // std::locale::global(std::locale("ru_RU.UTF-8"));
    // const auto loc {std::setlocale(LC_TIME, "ru_RU.UTF-8")};
    // if (!loc)
    //{
    //     std::setlocale(LC_TIME, current_locale);
    //     std::wcout << L"Failed to set locale\n";
    // }

    // std::string prev_loc = std::setlocale(LC_ALL, nullptr);
    //// Restore the previous locale.
    // if (const char* temp = std::setlocale(LC_ALL, prev_loc.c_str()))
    //     std::wprintf(L"Restorred LC_ALL locale: %s\n", temp);

    std::wcout.imbue(loc);
    const auto t {std::filesystem::last_write_time(target_path)};
    const auto system_time {
        std::chrono::clock_cast<std::chrono::system_clock>(t)};
    const std::wstring year {std::format(L"{:%Y}", t)};
    const std::wstring month {std::format(loc, L"{:%B}", t)};

    const std::time_t t2 {get_last_modification_time(target_path)};
    char mbstr[128] {};
    if (std::strftime(mbstr, sizeof(mbstr), "%B", std::localtime(&t2)))
    {
        const std::wstring str {string_to_wstring(mbstr)};
        std::wcout << str << L'\n';
        // std::tm tm = *std::localtime(&t2);
        // std::cout << std::put_time(&tm, "%c %Z") << '\n';
        // std::wcout << std::strftime(nullptr, 100, "%Y/%m_%B",
        //     std::localtime(&t2))<< L'\n';
    }

    std::wcout << target_path << L": last modified"
        << std::format(L": {}\n", t);
    std::wcout << target_path << L": last modified"
        << std::format(L": {:%c}\n", t);
    // std::wprintf(L"Text %ls\n", month);
    std::wcout << L"Year = " << year << L"; month = " << month << "\n";
}

static void print_target_path(const std::filesystem::path& dir_path)
{
    std::wcout << dir_path << "\n";
}

static void process_path_items(const std::filesystem::path& dir_path,
    const std::function<void(const std::filesystem::path&)>& process_function,
    const bool recursive = false)
{
    // Loop through items in the given directory.
    for (const std::filesystem::directory_entry& item :
        std::filesystem::directory_iterator {dir_path})
    {
        process_function(item.path());

        // Displaying items.
        if (item.is_directory() && recursive)
        {
            // If scanning recursively, then process the directory item.
            if (recursive)
            {
                process_path_items(item.path(), process_function, recursive);
            }
        }
    }
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
        process_path_items(target_path, print_target_path, recursive);
        process_path_items(target_path, print_last_write_time, recursive);
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
    for (std::string& option : options)
    {
        switch (option[1])
        {
            // Flag for recursion.
            case 'r':
                recursive = true;
                break;

            // Flags for locale.
            case 'l':
            case 'L':
                if (option.length() > 3)
                {
                    locale_name = option.substr(3);
                }
                break;

            default:
                break;
        }
    }

    for (const std::string& target_path_name : paths)
    {
        show_files_in_directory(target_path_name, recursive);
    }

    return 0;
}
