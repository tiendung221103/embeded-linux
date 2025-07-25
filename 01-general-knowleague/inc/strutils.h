#ifndef STRUTILS_H
#define STRUTILS_H

/**
 * @brief Reverses a given null-terminated string in place.
 * 
 * @param [in,out] str Pointer to the string to be reversed. 
 *                     The reversed string will overwrite the original.
 */
void str_reverse(char *str);

/**
 * @brief Removes leading and trailing whitespace characters from a string in place.
 * 
 * @param [in,out] str Pointer to the string to be trimmed.
 *                     The trimmed string will overwrite the original.
 */
void str_trim(char *str);

/**
 * @brief Converts a numeric string to an integer.
 * 
 * @param [in] str Pointer to the null-terminated string containing the number.
 * @param [out] out_num Pointer to an integer where the result will be stored if conversion is successful.
 * 
 * @return 0 on success, or a non-zero error code if conversion fails (e.g., invalid format).
 */
int str_to_int(const char *str, int *out_num);

#endif
