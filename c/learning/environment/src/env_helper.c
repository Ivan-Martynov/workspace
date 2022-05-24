#include <stdio.h>
#include <stdlib.h>

extern char **environ;

void display_variable(const char* const var_name)
{
    const char* const env_var = getenv(var_name);

    if (env_var != NULL)
    {
        printf("%s=%s\n", var_name, env_var);
    }
    else
    {
        printf("Environment variable %s not found\n", var_name);
    }
}

size_t list_variables()
{
    size_t count = 0;

    for (char* const* p = environ; *p != NULL; ++p)
    {
        printf("Environment vairable: %s\n", *p);
        ++count;
    }

    return count;
}
