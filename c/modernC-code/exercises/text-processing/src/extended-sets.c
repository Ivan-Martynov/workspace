#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <wchar.h>
#include <limits.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include <wctype.h>

typedef uint_least16_t char16_t;
typedef uint_least32_t char32_t;
typedef int (*wcclass_t)(wint_t);

#if 0
#ifndef __STDC_ISO_10646__
#error "wchar_t wide characters have to be Unicode code points."
#endif

#ifndef __STDC_MB_MIGHT_NEQ_WC__
#error "basic character codes must agree on char and wchar_t."
#endif
#endif

union mbstate_t_ {
    unsigned char bytes[sizeof(mbstate_t)];
    mbstate_t state;
};

#define MBSTATE (&(union mbstate_t_) {.bytes = {0}}.state)

enum
{
    g_l = 1,
    g_r = 2,
    g_u = 4,
    g_d = 8,
};

#define MONTH(X)                                                               \
    (char const*[]) {                                                          \
        [1] = "Jan",                                                           \
        [2] = "Feb",                                                           \
        [3] = "Mar",                                                           \
        [4] = "Apr",                                                           \
        [5] = "Mai",                                                           \
        [6] = "Jun",                                                           \
        [7] = "Jul",                                                           \
        [8] = "Aug",                                                           \
        [9] = "Sep",                                                           \
        [10] = "Oct",                                                          \
        [11] = "Nov",                                                          \
        [12] = "Dec",                                                          \
    }[(X) % 100]

#define STRINGIFY_(X) #X
#define STRINGIFY(X) STRINGIFY_(X)

#define YEAR(X)                                                                \
    (char const[5])                                                            \
    {                                                                          \
        STRINGIFY(X)[0], STRINGIFY(X)[1], STRINGIFY(X)[2], STRINGIFY(X)[3],    \
    }

#define VBAR "\u2502"
#define HBAR "\u2500"
#define TOPLEFT "\u250c"
#define TOPRIGHT "\u2510"

static const char* const g_table[] = {
    [0] = " ",
    [g_l] = "\u2574",
    [g_r] = "\u2576",
    [g_l | g_r] = "\u2500",
    [g_u] = "\u2575",
    [g_l | g_u] = "\u2518",
    [g_r | g_u] = "\u2514",
    [g_l | g_r | g_u] = "\u2534",
    [g_d] = "\u2577",
    [g_l | g_d] = "\u2510",
    [g_r | g_d] = "\u250c",
    [g_l | g_r | g_d] = "\u252c",
    [g_u | g_d] = "\u2502",
    [g_l | g_u | g_d] = "\u2524",
    [g_r | g_u | g_d] = "\u251c",
    [g_l | g_r | g_u | g_d] = "\u253c",
};

#define SURROG0 0xD800L
#define SURROG1 (SURROG0 + 1024)
#define SURROG2 (SURROG1 + 1024)

int iswhighsurrogate(wint_t x)
{
    return (SURROG0 <= x) && (x < SURROG1);
}

int iswlowsurrogate(wint_t x)
{
    return (SURROG1 <= x) && (x < SURROG2);
}

int iswsurrogate(wint_t x)
{
    return (SURROG0 <= x) && (x < SURROG2);
}

int iswvalid(wint_t x)
{
    return x && x != WEOF && !iswsurrogate(x);
}

/**
 * @brief Interpret a mb string in @mbs and return its length when interpreted
 * as a wide character string.
 *
 * @param mbs
 * @param state
 *
 * @return size_t The length of the mb string or -1 if an encoding error
 * occured. This function can be integrated into a sequence of searches through
 * a string, as long as a @state argument is passed to this function that is
 * consistent with the mb character starting in @mbs. The state itself is not
 * modified by this function.
 *
 * @remark @state of 0 indicated that @mbs can be scanned without considering
 * any context.
 *
 * @version 0.1
 *
 * @date 2024-03-13
 */
static size_t mbsrlen(const char* mbs, const mbstate_t* restrict state)
{
    if (!state)
    {
        state = MBSTATE;
    }

    mbstate_t st = *state;

    const size_t mblen = mbsrtowcs(NULL, &mbs, 0, &st);
    if (mblen == (size_t)-1)
    {
        errno = 0;
    }

    return mblen;
}

/**
 * @brief Interpret a sequence of bytes in @s as mb string and convert it to a
 * wide character string.
 *
 * @param s
 * @param state
 *
 * @return wchar_t* Newly malloc'ed wide character string of the appropriate
 * length, 0 if an encoding error occurred.
 *
 * @remark This function can be integrated into a sequence of such searches
 * through a string, as long as a @state argument is passed to this function
 * that is consistent with the mb character starting in @s. The state itself is
 * not modified by this function. @state of 0 indicated that @s can be scanned
 * without considering any context.
 *
 * @version 0.1
 *
 * @date 2024-03-13
 */
wchar_t* mbsrdup(const char* s, const mbstate_t* restrict state)
{
    const size_t mblen = mbsrlen(s, state);

    if (mblen == (size_t)-1)
    {
        return NULL;
    }

    mbstate_t st = state ? *state : *MBSTATE;

    wchar_t* result = malloc(sizeof(wchar_t[mblen + 1]));

    if (result)
    {
        mbsrtowcs(result, &s, mblen + 1, &st);
    }

    return result;
}

/**
 * @brief Interpret a sequence of bytes in @source as mb character and return as
 * wide character through @target.
 *
 * @param target
 * @param source
 * @param state
 *
 * @return size_t Length of the mb character or -1 in an encoding error
 * occurred. This function can be integrated into a sequence of such searches
 * through a string, as long as the same @state argument passed to all calls to
 * this or similar functions.
 *
 * @remark @state of 0 indicated that @source can be scanned without considering
 * any context.
 *
 * @version 0.1
 *
 * @date 2024-03-13
 */
size_t mbrtow(wchar_t* restrict target, const char source[restrict static 1],
    mbstate_t* restrict state)
{
    if (!state)
    {
        state = MBSTATE;
    }

    size_t len = -2;
    for (size_t maxlen = MB_LEN_MAX; len == (size_t)-2; maxlen *= 2)
    {
        len = mbrtowc(target, source, maxlen, state);
    }

    if (len == (size_t)-1)
    {
        errno = 0;
    }

    return len;
}

wint_t mbtow(char const* c)
{
    wchar_t C = 0;
    size_t len = mbrtow(&C, c, MBSTATE);
    return (len == (size_t)-1) ? WEOF : C;
}

/**
 * @brief Interpret a sequence of bytes in @s as mb string and search for wide
 * character @c.
 *
 * @param s
 * @param state
 * @param c
 * @param occurrence
 *
 * @return const char* The @occurrence'th position in @s that starts a mb
 * sequence corresponding to @c or 0 if an encoding error occurred. If the
 * number of occurrences is less than @occurrence the last such position is
 * returned. So in particular using SIZE_MAX (or - 1) will always return the
 * last occurrence.
 *
 * @remark This function can be integrated into a sequence of such searches
 * through a string, as long as a @state argument is passed to this or similar
 * functions and as long as continuation of the search starts at the position
 * that is returned by this function. @state of 0 indicates that @s can be
 * scanned without considering any context.
 *
 * @version 0.1
 *
 * @date 2024-03-13
 */
const char* mbsrwc(const char s[restrict static 1], mbstate_t* restrict state,
    wchar_t c, size_t occurrence)
{
#if 1
    if (!c)
#else
    if (!c || (c == WEOF))
#endif
    {
        return NULL;
    }

    if (!state)
    {
        state = MBSTATE;
    }

    const char* result = NULL;
    mbstate_t st = *state;

    for (size_t len = 0; s[0]; s += len)
    {
        mbstate_t backup = st;

        wchar_t wc = 0;
        len = mbrtow(&wc, s, &st);

        if (!wc)
        {
            break;
        }

        if (c == wc)
        {
            *state = backup;
            result = s;

            if (occurrence == 0)
            {
                break;
            }
            else
            {
                --occurrence;
            }
        }
    }

    return result;
}

/**
 * @brief Draw multibyte strings @start and @end separated by horizontal line.
 *
 * @param start
 * @param end
 *
 * @version 0.1
 *
 * @date 2024-03-13
 */
static void draw_sep(const char start[static 1], const char end[static 1])
{
    fputs(start, stdout);

    const size_t start_len = mbsrlen(start, 0);
    const size_t end_len = 90 - mbsrlen(end, 0);

    for (size_t i = start_len; i < end_len; ++i)
    {
        fputs(HBAR, stdout);
    }

    fputs(end, stdout);
    fputc('\n', stdout);
}

char const* mbsrmb(char const s[static 1], mbstate_t* restrict state,
    char const c[static 1], size_t occurrence)
{
    if (!state)
        state = MBSTATE;
    wint_t C = mbtow(c);
    return mbsrwc(s, state, C, occurrence);
}

char const* mbsrrwc(
    char const s[restrict static 1], mbstate_t* restrict state, wchar_t C)
{
    return mbsrwc(s, state, C, -1);
}

char const* mbsrrmb(
    char const s[static 1], mbstate_t* restrict state, char const c[static 1])
{
    return mbsrmb(s, state, c, -1);
}

char const* mbsrwcjump(char const s1[static 1], mbstate_t* restrict state,
    size_t S2len, wchar_t const S2[S2len])
{
    if (!state)
        state = MBSTATE;
    mbstate_t st = *state;
    for (size_t i = 0; i < S2len; ++i)
    {
        wchar_t S1 = 0;
        s1 += mbrtow(&S1, s1, &st);
        if (S1 != S2[i])
            return 0;
    }
    *state = st;
    return s1;
}

char const* mbsrwcs(char const s1[static 1], mbstate_t* restrict state,
    wchar_t const S2[static 1])
{
    if (!state)
        state = MBSTATE;
    size_t S2len = wcslen(S2);
    switch (S2len)
    {
        case 0:
            return 0;
        case 1:
            return mbsrwc(s1, state, S2[0], 0);
        default:;
            /* Don't modify shift state until we found it. */
            mbstate_t rstate = *state;
            while (s1 && s1[0])
            {
                s1 = mbsrwc(s1, &rstate, S2[0], 0);
                if (s1 && s1[0])
                {
                    /* s1 now is at a potential starting point */
                    char const* ret = s1;
                    mbstate_t tstate = rstate;
                    if (mbsrwcjump(s1, &tstate, S2len, S2))
                    {
                        *state = rstate;
                        return ret;
                    }
                    /* Advance s1 to the next mb character. */
                    s1 += mbrtow(0, s1, &rstate);
                }
            }
    }
    return 0;
}

char const* mbsrmbs(
    char const s1[static 1], mbstate_t* restrict state, char const s2[static 1])
{
    if (!state)
        state = MBSTATE;
    wchar_t* restrict S2 = mbsrdup(s2, 0);
    if (!S2 || !S2[0])
        return 0;
    s1 = mbsrwcs(s1, state, S2);
    free(S2);
    return s1;
}

char const* mbsrwcsskip(
    char const s1[static 1], mbstate_t* restrict state, wchar_t const* S2)
{
    if (S2)
    {
        if (!state)
            state = MBSTATE;
        mbstate_t st = *state;
        for (size_t len; s1[0]; *state = st, s1 += len)
        {
            wchar_t S1[3] = {0};
            len = mbrtow(&S1[0], s1, &st);
            if (!S1[0])
                break;
            if (!iswlowsurrogate(S1[0]))
            {
                if (!wcschr(S2, S1[0]))
                    break;
            }
            else
            {
                len += mbrtow(&S1[1], s1, &st);
                if (!wcsstr(S2, S1))
                    break;
            }
        }
        *state = st;
    }
    return s1;
}

char const* mbsrskip(
    char const s1[static 1], mbstate_t* restrict state, char const* s2)
{
    if (!state)
        state = MBSTATE;
    wchar_t* restrict S2 = mbsrdup(s2, 0);
    s1 = mbsrwcsskip(s1, state, S2);
    free(S2);
    return s1;
}

size_t mbsspn(char const* s1, char const* s2)
{
    return s1 ? mbsrskip(s1, 0, s2) - s1 : 0;
}

char const* mbsrskip_class(
    char const s1[static 1], mbstate_t* restrict state, wcclass_t func)
{
    if (!state)
        state = MBSTATE;
    for (size_t len; s1[0]; s1 += len)
    {
        wchar_t S1 = 0;
        len = mbrtow(&S1, s1, state);
        if (!S1 || !func(S1))
            break;
    }
    return s1;
}

size_t mbsspn_class(char const* s1, wcclass_t func)
{
    return s1 ? mbsrskip_class(s1, 0, func) - s1 : 0;
}

char const* mbsrskip_type(
    char const s1[static 1], mbstate_t* restrict state, wctype_t type)
{
    if (!state)
        state = MBSTATE;
    for (size_t len; s1[0]; s1 += len)
    {
        wchar_t S1 = 0;
        len = mbrtow(&S1, s1, state);
        if (!S1 || !iswctype(S1, type))
            break;
    }
    return s1;
}

size_t mbsspn_type(char const* s1, wctype_t type)
{
    return s1 ? mbsrskip_type(s1, 0, type) - s1 : 0;
}

size_t mbsspn_name(char const* s1, char const name[static 1])
{
    return mbsspn_type(s1, wctype(name));
}

char const* mbsrwcscskip(
    char const s1[static 1], mbstate_t* restrict state, wchar_t const* S2)
{
    if (!state)
        state = MBSTATE;
    if (S2)
    {
        mbstate_t st = *state;
        for (size_t len; s1[0]; s1 += len)
        {
            wchar_t S1[3] = {0};
            len = mbrtow(&S1[0], s1, &st);
            if (!S1[0])
                break;
            if (!iswlowsurrogate(S1[0]))
            {
                if (wcschr(S2, S1[0]))
                    break;
            }
            else
            {
                len += mbrtow(&S1[1], s1, &st);
                if (wcsstr(S2, S1))
                    break;
            }
        }
        *state = st;
    }
    return s1;
}

char const* mbsrcskip(char const* s1, mbstate_t* restrict state, char const* s2)
{
    if (!state)
        state = MBSTATE;
    wchar_t* restrict S2 = mbsrdup(s2, 0);
    s1 = mbsrwcscskip(s1, state, S2);
    free(S2);
    return s1;
}

size_t mbscspn(char const* s1, char const* s2)
{
    return (s1 && s2) ? mbsrcskip(s1, 0, s2) - s1 : 0;
}

double mbsrtod(
    char const* restrict s1, mbstate_t* restrict state, char** restrict endptr)
{
    if (!state)
        state = MBSTATE;
    return strtod(mbsrskip_class(s1, state, iswspace), endptr);
}

long double mbsrtold(
    char const* restrict s1, mbstate_t* restrict state, char** restrict endptr)
{
    if (!state)
        state = MBSTATE;
    return strtold(mbsrskip_class(s1, state, iswspace), endptr);
}

long mbsrtol(char const* restrict s1, mbstate_t* restrict state,
    char** restrict endptr, int base)
{
    if (!state)
        state = MBSTATE;
    return strtol(mbsrskip_class(s1, state, iswspace), endptr, base);
}

long long mbsrtoll(char const* restrict s1, mbstate_t* restrict state,
    char** restrict endptr, int base)
{
    if (!state)
        state = MBSTATE;
    return strtoll(mbsrskip_class(s1, state, iswspace), endptr, base);
}

unsigned long mbsrtoul(char const* restrict s1, mbstate_t* restrict state,
    char** restrict endptr, int base)
{
    if (!state)
        state = MBSTATE;
    return strtoul(mbsrskip_class(s1, state, iswspace), endptr, base);
}

unsigned long long mbsrtoull(char const* restrict s1, mbstate_t* restrict state,
    char** restrict endptr, int base)
{
    if (!state)
        state = MBSTATE;
    return strtoull(mbsrskip_class(s1, state, iswspace), endptr, base);
}

char* mbsrmbsncpy(size_t n, char t[restrict n], mbstate_t const* restrict state,
    char const s[restrict static 1])
{
    /* Don't do anything if s wouldn't fit entirely into t */
    char const* ep = memchr(s, 0, n);
    if (!ep)
        return 0;
    size_t slen = ep - s;
    /* If the target is in non-initial state, try to reset it. */
    if (!mbsinit(state))
    {
        /* Now state is known to be non-null. */
        char buf[2 * MB_LEN_MAX];
        mbstate_t st = *state;
        size_t len = wcrtomb(buf, 0, &st);
        /* See if shift characters plus s will fit into t. */
        if (slen + len >= n)
            return 0;
        /* write the bytes that end the shift state */
        memcpy(t, buf, len);
        t += len - 1;
        n -= len - 1;
    }
    if (slen >= n)
        return 0;
    memcpy(t, s, slen + 1);
    return t + slen;
}

char* mbsrncpy(size_t n, char t[restrict n], mbstate_t* restrict tstate,
    char const s[restrict static 1], mbstate_t const* restrict sstate)
{
    /* First check if the bytes can just be copied over. */
    if (mbsinit(sstate))
    {
        char* ret = mbsrmbsncpy(n, t, tstate, s);
        if (ret)
            return ret;
    }
    char* tt = t;
    if (!tstate)
        tstate = MBSTATE;
    if (!sstate)
        sstate = MBSTATE;
    mbstate_t sst = *sstate;
    char buf[2 * MB_LEN_MAX];
    for (size_t slen = 0; s[0] && n; s += slen)
    {
        wchar_t S[2] = {0};
        slen = mbrtowc(S, s, n, &sst);
        /* Here, wcsrtombs can't fail, but because of the different state
           of t, tlen may be larger than slen. */
        wchar_t const* Sp = S;
        mbstate_t tst = *tstate;
        size_t tlen = wcsrtombs(buf, &Sp, n, &tst);
        if (tlen > n)
            break;
        *tstate = tst;
        memcpy(tt, buf, tlen);
        if (!S[0])
            break;
        tt += tlen;
        n -= tlen;
    }
    return tt;
}

static void mb_test()
{
    char const* locC_ = setlocale(LC_CTYPE, 0);
    char locC[strlen(locC_) + 1];
    strcpy(locC, locC_);

    setlocale(LC_ALL, "");
    /* Multibyte character printing only works after the locale
       has been switched. */
    draw_sep(TOPLEFT " © 2014 jɛnz ˈgʊzˌtɛt ", TOPRIGHT);

    char const* locLoc_ = setlocale(LC_CTYPE, 0);
    char locLoc[strlen(locLoc_) + 1];
    strcpy(locLoc, locLoc_);
    char const strhello[] = "Hä\u0131łłö";
    char const u8hello[] = u8"Hä\u0131łłö";
    wchar_t const wcshello[] = L"Hä\u0131łłö";
    char const strvowel[] = "aeiouäëïöüáéíóúàèìòùæœ\u0131";
    char const u8vowel[] = u8"aeiouäëïöüáéíóúàèìòùæœ\u0131";
    wchar_t const wcsvowel[] = L"aeiouäëïöüáéíóúàèìòùæœ\u0131";
    char16_t c16butterfly[] = u"\U000272CA";
    char32_t c32butterfly[] = U"\U000272CA";
    wchar_t wbutterfly[] = L"\U000272CA";
    char u8butterfly[] = u8"\U000272CA";
    bool mbs_u8 = (sizeof strvowel == sizeof u8vowel)
                  && (!strcmp(strvowel, u8vowel))
                  && (!strcmp("\U000272CA", u8"\U000272CA"));
    printf(VBAR "%ls world: switched \"%s\" \u21E8 \"%s\", multibyte strings "
                "are%s utf8, state%s\n",
        wcshello, locC, locLoc, mbs_u8 ? "" : " not",
        mblen(0, 0) ? "full" : "less");
    printf(VBAR
        "wchar_t (%ssigned) is %zu bit, wint_t (%ssigned) is %zu bit, WEOF is ",
        (WCHAR_MIN ? "" : "un"), sizeof(wchar_t) * CHAR_BIT,
        (WINT_MIN ? "" : "un"), sizeof(wint_t) * CHAR_BIT);
    if (WINT_MIN)
        printf("%lld\n", (long long)WEOF);
    else
        printf("%#llx\n", (unsigned long long)WEOF);
    printf(VBAR "wchar_t are Unicode code points as of %s %s, ",
#ifdef __STDC_ISO_10646__
        MONTH(__STDC_ISO_10646__), YEAR(__STDC_ISO_10646__)
#else
        "<unknown>", "<unknown>"
#endif
    );
#ifdef __STDC_MB_MIGHT_NEQ_WC__
    fputs("the basic character set is not extended to wchar_t\n", stdout);
#else
    fputs("wchar_t extends the basic character set\n", stdout);
#endif
#ifdef __STDC_UTF_16__
    fputs(VBAR "char16_t is UTF-16, ", stdout);
#else
    fputs("char16_t is not UTF-16, ", stdout);
#endif
#ifdef __STDC_UTF_32__
    fputs("char32_t is UTF-32\n", stdout);
#else
    fputs(VBAR "char32_t is not UTF-32\n", stdout);
#endif
    draw_sep(g_table[g_u | g_d | g_r], g_table[g_l | g_u | g_d]);
    printf(VBAR "testing character %ls, codepoint U+%lX, utf8 %hhx %hhx %hhx "
                "%hhx %hhx:\n",
        wbutterfly, 0x272CALU, u8butterfly[0], u8butterfly[1], u8butterfly[2],
        u8butterfly[3], u8butterfly[4]);
    printf(VBAR "\twchar_t,\tlength %zu,\t%s\n",
        (sizeof(wbutterfly) / sizeof(wbutterfly[0])) - 1,
        (sizeof(wbutterfly) / sizeof(wbutterfly[0])) == 2
            ? "one word encoding"
            : "surrogate encoding");
    printf(VBAR "\tchar32_t,\tlength %zu,\t%s\n",
        (sizeof(c32butterfly) / sizeof(c32butterfly[0])) - 1,
        (sizeof(c32butterfly) / sizeof(c32butterfly[0])) == 2
            ? "one word encoding"
            : "surrogate encoding");
    printf(VBAR "\tchar16_t,\tlength %zu,\t%s\n",
        (sizeof(c16butterfly) / sizeof(c16butterfly[0])) - 1,
        (sizeof(c16butterfly) / sizeof(c16butterfly[0])) == 2
            ? "one word encoding"
            : "surrogate encoding");
    if (sizeof(wchar_t) == sizeof(char32_t))
        printf(VBAR "\twchar_t and char32_t encoding %s\n",
            (sizeof(c32butterfly) == sizeof(wbutterfly)
                && c32butterfly[0] == wbutterfly[0]
                && c32butterfly[1] == wbutterfly[1])
                ? "are equal"
                : "differ");
    else if (sizeof(wchar_t) == sizeof(char16_t))
        printf(VBAR "\twchar_t and char16_t encoding %s\n",
            (sizeof(c16butterfly) == sizeof(wbutterfly)
                && c16butterfly[0] == wbutterfly[0]
                && c16butterfly[1] == wbutterfly[1])
                ? "are equal"
                : "differ");
    else
        printf(VBAR "\twchar_t, char32_t and char16_t types and encodings all "
                    "differ\n");
    char const mbs0[] = u8"ł";
    char const* pos0 = mbsrmb(u8hello, 0, mbs0, 0);
    draw_sep(g_table[g_u | g_d | g_r], g_table[g_l | g_u | g_d]);
    printf(VBAR "search first:\tfound \"%s\",\tposition %td, %s\n", mbs0,
        pos0 - u8hello, pos0);
    pos0 = mbsrmb(u8hello, 0, mbs0, 1);
    printf(VBAR "search second:\tfound \"%s\",\tposition %td, %s\n", mbs0,
        pos0 - u8hello, pos0);
    char const mbs1[] = u8"ł";
    char const* pos1 = mbsrrmb(u8hello, 0, mbs1);
    printf(VBAR "search last:\tfound \"%s\",\tposition %td, %s\n", mbs1,
        pos1 - u8hello, pos1);
    char const mbs2[] = u8"łö";
    char const* pos2 = mbsrmbs(u8hello, 0, mbs2);
    printf(VBAR "search needle:\tfound \"%s\",\tposition %td, %s\n", mbs2,
        pos2 - u8hello, pos2);
    char const mbs3[] = u8" \u2003\u2002—";
    size_t pos3 = mbsspn_class(mbs3, iswspace);
    printf(VBAR "skip space:\tfound \"%s\",\tposition %zu, %s\n", mbs3, pos3,
        mbs3 + pos3);
    draw_sep(g_table[g_u | g_d | g_r], g_table[g_l | g_u | g_d]);
    printf(VBAR "floats (locale):\t%g (C with .)\t%g (C with ,)\t%g (with "
                ".)\t%g (with ,)\n",
        strtod("9.3", 0), strtod("9,3", 0), mbsrtod("\u20039.3", 0, 0),
        mbsrtod("\u20039,3", 0, 0));
    draw_sep(g_table[g_u | g_d | g_r], g_table[g_l | g_u | g_d]);
    printf(VBAR "vowels\t%ls\n", wcsvowel);
    char u8vowel2[MB_LEN_MAX + sizeof u8vowel];
    char* partial = mbsrncpy(sizeof u8vowel - 2, u8vowel2, 0, u8vowel, 0);
    if (partial[0])
        partial[0] = 0;
    printf(VBAR "partial\t%s\n", u8vowel2);
    for (char const* s = strhello; s && s[0];)
    {
        s += mbscspn(s, strvowel);
        int vowels = mbsspn(s, strvowel);
        if (!vowels)
        {
            printf("bizarre, no vowels in remaining string %s", s);
            break;
        }
        printf(VBAR "%d bytes for vowel(s) %.*s\n", vowels, vowels, s);
        s += vowels;
    }
    draw_sep(g_table[g_u | g_r], g_table[g_l | g_u]);
}

int main()
{
    mb_test();

    return EXIT_SUCCESS;
}
