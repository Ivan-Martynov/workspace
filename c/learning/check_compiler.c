#include <locale.h>
#include <stdio.h>
#include <assert.h>
#include <wchar.h>

#if __STDC_VERSION__ >= 202000L
#pragma message "C23 version."
#define null_pointer nullptr
#else
#pragma message "C23 nullptr is not supported."
#define null_pointer ((void*)0)
#if __STDC_VERSION__ >= 201710L
#pragma message "C17 version."
#elif __STDC_VERSION__ >= 201112L
#pragma message "C11 version."
#elif __STDC_VERSION__ >= 199901L
#pragma message "C99 version."
#else
#pragma message "C89/90 version."
#endif
#endif

int main()
{
    setlocale(LC_ALL, "");

    int* p = nullptr;
    static_assert(2 + 2 * 2 + 3 == 9);
    wprintf(L"STDC version = %ld\n", __STDC_VERSION__);
    wprintf(L"Check null pointer => %ls\n", (p == nullptr) ? L"true" : L"false");

    return 0;
}
