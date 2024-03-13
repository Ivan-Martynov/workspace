#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Print a series of numbers @numbers in @buffer, using printf format
 * @form, separated by @sep characters and terminated with a newline character.
 *
 * @param total
 * @param buffer
 * @param form
 * @param sep
 * @param len
 * @param numbers
 *
 * @return int Number of characters printed to @buffer. This supposes that
 * @total and @buffer are big enough and that @form is a format suitable to
 * print size_t.
 *
 * @version 0.1
 *
 * @date 2024-03-13
 */
static int sprintnumbers(size_t total, char buffer[restrict total],
    const char form[restrict static 1], const char sep[restrict static 1],
    size_t len, size_t numbers[restrict len])
{
    char* p = buffer; // Next position on the buffer.
    const size_t seplen = strlen(sep);

    if (len)
    {
        size_t i = 0;
        while (1)
        {
            p += sprintf(p, form, numbers[i]);
            if (++i >= len)
            {
                break;
            }

            memcpy(p, sep, seplen);
            p += seplen;
        }
    }

    memcpy(p, "\n", 2);

    return (p - buffer) + 1;
}

/**
 * @brief Read one text line of at most @size-1 bytes. The '\n' character is
 * replaced by 0.
 *
 * @param size
 * @param buffer
 * @param stream
 *
 * @return char* @buffer if an entire line was read successfully. Otherwise,
 * 0 is returned and @buffer contains a maximal partial line that could be
 * read.
 * @buffer is null termintated.
 *
 * @version 0.1
 *
 * @date 2024-03-13
 */
static char* fgetline(
    size_t size, char buffer[restrict size], FILE* restrict stream)
{
    buffer[0] = '\0';

    char* result = fgets(buffer, size, stream);

    if (result)
    {
        // buffer is writable so can be pos.
        char* pos = strchr(buffer, '\n');

        if (pos)
        {
            *pos = '\0';
        }
        else
        {
            result = NULL;
        }
    }

    return result;
}

static size_t numberline_inner(
    const char* restrict act, size_t numbers[restrict], int base)
{
    size_t n = 0;

    for (char* next = NULL; act[0]; act = next)
    {
        numbers[n] = strtoull(act, &next, base);
        if (act == next)
        {
            break;
        }
        ++n;
    }

    return n;
}

/**
 * @brief Interpret string @buffer as a sequence of number represented by @base.
 *
 * @param size
 * @param buffer Supposed to be a string.
 * @param np If non-null, the count of numbers is stored in *@np.
 * @param base Value from 0 to 36 with the same interpretation as for strtoul.
 *
 * @remark The caller of this function is reponsible to free the array that is
 * returned.
 *
 * @return size_t* Newly allocated array of numbers.
 *
 * @version 0.1
 *
 * @date 2024-03-13
 */
static size_t* numberline(size_t size, const char buffer[restrict size],
    size_t* restrict np, int base)
{
    size_t* result = NULL;

    size_t n = 0;

    // Check for validity of the string.
    if (memchr(buffer, '\0', size))
    {
        // The maximum number of integers encoded. To see that this may be as
        // much look at the sequence 08 08 08 08 ... and suppose that base is 0.
        result = malloc(sizeof(size_t[1 + (size << 1) / 3]));

        n = numberline_inner(buffer, result, base);

        // Supposes that shrinking realloc will always succeed.
        result = realloc(result, sizeof(size_t[n > 0 ? n : 1]));
    }

    if (np)
    {
        *np = n;
    }

    return result;
}

static int input_to_normalized()
{
    char buffer[256];
    while (1)
    {
        if (fgetline(sizeof(buffer), buffer, stdin))
        {
            size_t n;
            size_t* numbers = numberline(strlen(buffer) + 1, buffer, &n, 0);

            const int result = fprintnumbers(stdout, "%#zX", ",\t", n, numbers);

            if (result < 0)
            {
                return EXIT_FAILURE;
            }
        }
        else if (buffer[0]) // A partial line has been read.
        {
            while (1)
            {
                const int c = getc(stdin);

                if (c == EOF)
                {
                    return EXIT_FAILURE;
                }
                else if (c == '\n')
                {
                    fprintf(stderr, "Line too long: %s\n", buffer);
                    break;
                }
            }
        }
        else
        {
            break;
        }
    }
}

int main()
{
    input_to_normalized();

    return EXIT_SUCCESS;
}
