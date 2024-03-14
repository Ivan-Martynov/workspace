#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#ifndef EFAULT
#define EFAULT EDOM
#endif

#ifndef EOVERFLOW
#define EOVERFLOW (EFAULT - EOF)
#if EOVERFLOW > INT_MAX
#error EOVERFLOW constant is too large
#endif
#endif

#ifndef ENOMEM
#define ENOMEM (EOVERFLOW + EFAULT - EOF)
#if ENOMEM > INT_MAX
#error ENOMEM constant is too large
#endif
#endif

static inline int error_cleanup(int err, int prev)
{
    errno = prev;
    return -err;
}

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

int fprintnumbers_opt(FILE* restrict stream, const char form[restrict static 1],
    const char sep[restrict static 1], size_t len, size_t numbers[restrict len])
{
    if (!stream || (len && !numbers))
    {
        return -EFAULT;
    }
    else if (len > INT_MAX)
    {
        return -EOVERFLOW;
    }

    int err = errno;
    const size_t seplen = strlen(sep);

    size_t total = 0;
    size_t mtot = len * (seplen + 10);

    char* buffer = malloc(mtot);
    if (!buffer)
    {
        return error_cleanup(ENOMEM, err);
    }

    // Count the chars for the numbers.
    for (size_t i = 0; i < len; ++i)
    {
        total += snprintf(&buffer[total], 0, form, numbers[i]);
        if (++i >= len)
        {
            break;
        }

        if (total > mtot - 20)
        {
            mtot <<= 1;

            char* rep_buf = realloc(buffer, mtot);

            if (rep_buf)
            {
                buffer = rep_buf;
            }
            else
            {
                total = error_cleanup(ENOMEM, err);
                goto CLEANUP_LABEL;
            }
        }

        memcpy(&buffer[total], sep, seplen);
        total += seplen;

        if (total > INT_MAX)
        {
            total = error_cleanup(EOVERFLOW, err);
            goto CLEANUP_LABEL;
        }
    }

    buffer[total] = '\0';

    // Print the entire line in one go.
    if (fputs(buffer, stream) == EOF)
    {
        total = EOF;
    }

CLEANUP_LABEL:
    free(buffer);

    return total;
}

/**
 * @brief Print a series of numbers @numbers on @stream, using printf format
 * @form separated by @sep characters and terminated with a newline character.
 *
 * @param stream
 * @param format
 * @param sep
 * @param len
 * @param numbers
 *
 * @return int Number of characters printed to @stream, or a negative error
 * value on error. If @len is 0 an empty line is printed and 1 is returned.
 * Possible error returns are:
 *      - EOF (which is negative) if @stream was not ready to be written to.
 *      - EOVERFLOW if more than INT_MAX characters would have to be written,
 *        including the case than @len is greater than INT_MAX.
 *      - EFAULT if @stream or @numbers are 0.
 *      - ENOMEM if a memory error occured.
 * This function leaves errno to the same value as occurred on entry.
 *
 * @version 0.1
 *
 * @date 2024-03-14
 */
int fprintnumbers(FILE* restrict stream, const char form[restrict static 1],
    const char sep[restrict static 1], size_t len, size_t numbers[restrict len])
{
    if (!stream || (len && !numbers))
    {
        return -EFAULT;
    }
    else if (len > INT_MAX)
    {
        return -EOVERFLOW;
    }

    size_t total = (len > 0 ? len : 1) * strlen(sep);
    int err = errno;

    // Count the chars for the numbers.
    for (size_t i = 0; i < len; ++i)
    {
        total += snprintf(NULL, 0, form, numbers[i]);
    }

    // We return int so we have to constrain the max size.
    if (total > INT_MAX)
    {
        return error_cleanup(EOVERFLOW, err);
    }

    char* buffer = malloc(total + 1);
    if (!buffer)
    {
        return error_cleanup(ENOMEM, err);
    }

    sprintnumbers(total, buffer, form, sep, len, numbers);
    // Print the entire line in one go.
    if (fputs(buffer, stream) == EOF)
    {
        total = EOF;
    }

    free(buffer);

    return total;
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

#if 1
            const int result
                = fprintnumbers_opt(stdout, "%#zX", ",\t", n, numbers);
#else
            const int result = fprintnumbers(stdout, "%#zX", ",\t", n, numbers);
#endif

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

    return EXIT_SUCCESS;
}

int main()
{
    input_to_normalized();

    return EXIT_SUCCESS;
}
