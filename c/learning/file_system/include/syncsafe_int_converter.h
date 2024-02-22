#ifndef _H_SYNCSAFE_INT_CONVERTER_H_
#define _H_SYNCSAFE_INT_CONVERTER_H_

#include <stddef.h>

/**
 * @brief Encode value as a syncsafe integer.
 */
size_t syncsafe_encode(size_t);

/**
 * @brief Decoding syncsafe value.
 */
size_t syncsafe_decode(const size_t);

/**
 * @brief Bytes to integer.
 */
size_t bytes_to_size_t(const char* const, const size_t);

#endif //_H_SYNCSAFE_INT_CONVERTER_H_
