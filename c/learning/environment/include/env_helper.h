#ifndef ENV_HELPER_H
#define ENV_HELPER_H

#include <stdlib.h>

/**
 * @brief Try to find and print an environment variable by its name.
 * 
 * @param var_name Name of the environment variable.
 */
void display_variable(const char* const var_name);

/**
 * @brief Display all found environment variables and count their number.
 * 
 * @return Number of found environment variables.
 */
size_t list_variables();

#endif // ENV_HELPER_H
