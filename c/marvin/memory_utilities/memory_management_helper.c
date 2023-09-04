#include "../type_utilities/type_constants.h"
#include "memory_management_helper.h"

#include <stdlib.h>

/**
 * @brief Deallocate and set pointer to null.
 *
 * @param ptr_ptr Pointer to pointer to free and reset.
 *
 * @version 0.1
 *
 * @date 2023-09-04
 */
void mrvn_free_set_null(void** ptr_ptr)
{
    if (ptr_ptr && *ptr_ptr)
    {
        free(*ptr_ptr);
        *ptr_ptr = mrvn_null_ptr;
    }
}
