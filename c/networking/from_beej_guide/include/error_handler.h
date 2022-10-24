#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

/**
 * @brief Get the addrinfo error code to string object.
 * 
 * @param status Result of the getaddrinfo function call.
 * @return char* String representing the error.
 */
char* getaddrinfo_error_code_to_string(const int);

/**
 * 
 * 
 */
/**
 * @brief Print an error message and reset the errno value.
 * 
 * @param message Text to prepend the strerror(errno) message.
 * 
 * @return int Number of characters transmitted to the error output stream.
 */
int report_error(const char* const message);

#endif //ERROR_HANDLER_H
