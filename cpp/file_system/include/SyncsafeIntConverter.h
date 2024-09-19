#ifndef _H_SYNCSAFE_INT_CONVERTER_H_
#define _H_SYNCSAFE_INT_CONVERTER_H_

#include <cstddef>

namespace Marvin
{

class SyncsafeIntConverter
{
  private:
    SyncsafeIntConverter() = delete;

  public:
    /**
     * @brief Bytes to integer.
     *
     * @param[in] bytes Bytes representing the value.
     * @param[in] size Number of bytes.
     *
     * @return size_t Calculated value.
     */
    static size_t bytes_to_size_t(const char* const, const size_t);

    /**
     * @brief Encode value as a syncsafe integer.
     *
     * @param[in] value Value to encode.
     *
     * @return size_t Encoded value.
     */
    static size_t syncsafe_encode(size_t value);

    /**
     * @brief Decode value as a syncsafe integer.
     *
     * @param[in] value Value to decode.
     *
     * @return size_t Decoded value.
     */
    static size_t syncsafe_decode(const size_t);

    /**
     * @brief Decode value as a syncsafe integer,converting bytes to integer.
     *
     * @param[in] bytes Bytes representing the value.
     * @param[in] size Number of bytes.
     *
     * @return size_t Decoded value.
     */
    static size_t syncsafe_decode_from_bytes(const char* const, const size_t);
};

} // namespace Marvin

#endif // _H_SYNCSAFE_INT_CONVERTER_H_
