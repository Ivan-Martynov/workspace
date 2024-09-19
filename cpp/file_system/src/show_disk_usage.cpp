#include <iostream>
#include <vector>
#include <filesystem>

/**
 * @brief Calculate percentage of occupied space.
 * 
 * @param info Information about space.
 * 
 * @return std::uintmax_t Percent value.
 * 
 * @version 0.1
 * 
 * @date 2024-06-28
 */
static std::uintmax_t calculate_usage_percentage(
    const std::filesystem::space_info& info)
{
    // Check if information is valid.
    if (constexpr std::uintmax_t invalid {static_cast<std::uintmax_t>(-1)};
        (info.free == 0) || (info.capacity == 0) || (info.available == 0)
        || (info.free == invalid) || (info.capacity == invalid)
        || (info.available == invalid))
    {
        return 100;
    }

    // Use actual usable space information.
    const std::uintmax_t space {info.capacity - (info.free - info.available)};

    return static_cast<double>(space - info.available) / space * 100;
}

/**
 * @brief Process all paths.
 * 
 * @param paths Paths to process.
 * 
 * @version 0.1
 * 
 * @date 2024-06-28
 */
static void show_disk_usage(const std::vector<std::filesystem::path>& paths)
{
    if (paths.empty())
    {
        return;
    }

    constexpr int width {14};

    std::wcout << std::left;
    for (const auto& s : {L"Path", L"Capacity", L"Free", L"Available",
        L"Used (%)"})
    {
        std::wcout << L"| " << std::setw(width) << s;
    }
    std::wcout << '\n';

    for (const auto& target_path: paths)
    {
        const std::filesystem::space_info info {
            std::filesystem::space(target_path)};

        std::wcout << L"| " << std::setw(width) << target_path;
        for (const auto x: {info.capacity, info.free, info.available,
            calculate_usage_percentage(info)})
        {
            std::wcout << L"| " << std::setw(width) << static_cast<intmax_t>(x);
        }
        std::wcout << L'\n';
    }
}

int main(const int argc, const char* argv[])
{
    std::setlocale(LC_ALL, "");

    std::vector<std::filesystem::path> paths {};
    for (int i {1}; i < argc; ++i)
    {
        if (const auto target_path {
                std::filesystem::path {argv[i]}.make_preferred()};
            std::filesystem::exists(target_path))
        {
            paths.emplace_back(target_path);
        }
    }

    show_disk_usage(paths);
 
    return 0;
}