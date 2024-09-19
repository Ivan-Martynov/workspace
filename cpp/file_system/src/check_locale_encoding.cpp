/**
 * @file check_locale_encoding.cpp
 *
 * @brief Program is run with argument $(locale -a) and outputs the encodings
 * associated with all available locales in Linux.
 *
 * @version 0.1
 *
 */
#include <iostream>
#include <locale>
#include <langinfo.h>

int main(const int argc, const char* argv[])
{
    for (int i {1}; i < argc; ++i)
    {
        if (std::setlocale(LC_ALL, argv[i]))
        {
            const auto code {nl_langinfo(CODESET)};
            if (code)
            {
                std::cout << argv[i] << " => " << code << "\n";
            }
            else
            {
                std::cout << "? " << argv[i] << " (nl_langinfo)\n";
            }
        }
        else
        {
            std::cout << "? " << argv[i] << " (setlocale)\n";
        }
    }

    return 0;
}