#include <filesystem>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>

struct TAGdata
{
    char tag[3];
    char title[30];
    char artist[30];
    char album[30];
    char year[4];
    char comment[30];
    int genre;
};

void test_directory()
{
    const std::string directory_path {
        "C:/Users/Ivan/Downloads/books/Папа, мама, бабушка, восемь детей и "
        "грузовик/1 Папа, мама, восемь детей и грузовик/"};
    const std::filesystem::path dir_path {directory_path};

    for (const std::filesystem::directory_entry& item :
        std::filesystem::directory_iterator {dir_path})
    {
        //std::cout << item.path().string() << "\n";
        //std::cout << item.path().stem() << "\n";
        //std::cout << item.path().extension() << "\n";

        std::ifstream file_handle {
            item.path().string(), std::ios::binary | std::ios::in};
        if (file_handle)
        {
            std::cout << "File " << item.path().string()
                      << " successfully opened.\n";

#if 0
            constexpr size_t mp3_tag_size {128};
            file_handle.seekg(-mp3_tag_size, std::ios::end);
            if (file_handle.fail())
            {
                std::cout << "Failed seeking.\n";
                continue;
            }
#endif

#if 1
            char test[4] = "\0";
            file_handle.read(test, 3);
            // std::string str {};
            // file_handle >> str;
#else
            TAGdata tagStruct {};
            file_handle.read(reinterpret_cast<char*>(&tagStruct), mp3_tag_size);
#endif
            if (file_handle.fail())
            {
                std::cout << "Failed reading.\n";
                continue;
            }

            std::cout << test << "\n";
            // strcpy(test, "\0");
            for (size_t i {0}; i < 2; ++i)
            {
                char tag {};
                file_handle >> tag;
                std::cout << static_cast<int>(tag) << "\n";
            }

            file_handle.read(test, 4);
            for (size_t i {0}; i < 4; ++i)
            {
                std::cout << static_cast<int>(test[i]) << " ";
            }
            const bool is_footer_present {static_cast<int>(test[3]) == 1};
            std::cout << test << "\n";

            const size_t n {is_footer_present ? 20UL : 10UL};
            for (size_t i {0}; i < n; ++i)
            {
                char tag {};
                file_handle >> tag;
                std::cout << tag << " " << static_cast<int>(tag) << "\n";
            }

            file_handle.close();

            // std::cout << tagStruct.tag << " => " << tagStruct.title << " => "
            //          << tagStruct.artist << " => " << tagStruct.album << " =>
            //          "
            //          << tagStruct.year << " => " << tagStruct.comment << " =>
            //          "
            //          << tagStruct.genre << "\n";
        }
    }
}

int main()
{
    test_directory();

    return 0;
}
