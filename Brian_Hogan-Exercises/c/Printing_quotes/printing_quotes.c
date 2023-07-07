#define __STDC_WANT_LIB_EXT1__ 1

#include <stdio.h>
#include <string.h>

/*
Quotation marks are often used to denote the start and end
of a string. But sometimes we need to print out the quotation
marks themselves by using escape characters.
Create a program that prompts for a quote and an author.
Display the quotation and author as shown in the example
output.

Example Output
What is the quote? These aren't the droids you're looking for.
Who said it? Obi-Wan Kenobi
Obi-Wan Kenobi says, "These aren't the droids
you're looking for."

Constraints:

- Use a single output statement to produce this output,
using appropriate string-escaping techniques for quotes.

- If your language supports string interpolation or string
substitution, don’t use it for this exercise. Use string
concatenation instead.

Challenge
- In Chapter 7, Data Structures, on page 63, you’ll practice
working with lists of data. Modify this program so that
instead of prompting for quotes from the user, you create
a structure that holds quotes and their associated
attributions and then display all of the quotes using the
format in the example. An array of maps would be a
good choice.
*/

int main()
{
    printf("What is the quote? ");

    const size_t buf_size = 256;

    char quote[buf_size];
    gets(quote);

    printf("Who said it? ");

    char author[buf_size];
    gets(author);
    
    printf("%s says, \"%s\"\n", author, quote);

    return 0;
}