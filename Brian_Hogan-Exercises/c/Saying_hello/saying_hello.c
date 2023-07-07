#include <stdio.h>
#include <stdlib.h>

/*
Create a program that prompts for your name and prints a greeting using your
name.

Example Output
What is your name? Brian
Hello, Brian, nice to meet you!

 Constraint:
- Keep the input, string concatenation, and output separate.

Challenges:

- Write a new version of the program without using any
variables.

- Write a version of the program that displays different
greetings for different people. This would be a good
challenge to try after you’ve completed the exercises in
Chapter 4, Making Decisions, on page 29 and Chapter 7,
Data Structures, on page 63.

 */

int main(int argc, char* argv[argc + 1])
{
    printf("What is your name? ");

    printf("Hello, %s, nice to meet you!\n", gets(*argv));

    return 0;
}
