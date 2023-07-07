#include <stdio.h>
#include <string.h>

/*
Create a program that prompts for an input string and displays
output that shows the input string and the number of
characters the string contains.

Example Output
What is the input string? Homer
Homer has 5 characters.

Constraints:

- Be sure the output contains the original string.

- Use a single output statement to construct the output.

- Use a built-in function of the programming language to
determine the length of the string.

Challenges
- If the user enters nothing, state that the user must enter
something into the program.

- Implement this program using a graphical user interface
and update the character counter every time a key is
pressed. If your language doesn’t have a particularly
friendly GUI library, try doing this exercise with HTML
and JavaScript instead.
*/

int main()
{
    printf("What is the input string? ");

    char str[256];
    #if 0
    scanf("%s\n", str);
    #else

    const char* p = fgets(str, 256, stdin);
    #endif

    const size_t length = strlen(str);

    if (length > 0)
    {
        printf("%s has %zu character%s", p, length,
            (length == 1) ? ".\n" : "s.\n");
    }
    else
    {
        printf("Some text should be entered.");
    }

    return 0;
}