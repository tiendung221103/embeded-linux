#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "strutils.h"

/**
 * @brief Reverses a null-terminated string in place.
 *
 * @param [in,out] str Pointer to the string to be reversed.
 *                     The original string is modified.
 */
void str_reverse(char *str) {
    if (!str) return;
    int len = strlen(str);
    for (int i = 0; i < len / 2; ++i) {
        char tmp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = tmp;
    }
}

/**
 * @brief Removes leading and trailing whitespace characters from a string in place.
 *
 * @param [in,out] str Pointer to the string to be trimmed.
 *                     The original string is modified.
 */
void str_trim(char *str) {
    if (!str) return;

    // Remove leading whitespace
    int start = 0;
    while (isspace((unsigned char)str[start])) start++;

    // Remove trailing whitespace
    int end = strlen(str) - 1;
    while (end > start && isspace((unsigned char)str[end])) end--;

    // Shift trimmed content to the beginning of the string
    int i, j = 0;
    for (i = start; i <= end; i++, j++) {
        str[j] = str[i];
    }
    str[j] = '\0';
}

/**
 * @brief Converts a numeric string to an integer.
 *
 * @param [in] str Pointer to the null-terminated string representing the number.
 * @param [out] out_num Pointer to an integer where the result will be stored if successful.
 *
 * @return 1 if conversion is successful (valid integer), 0 otherwise.
 */
int str_to_int(const char *str, int *out_num) {
    if (!str || !out_num) return 0;
    char *endptr;
    long val = strtol(str, &endptr, 10);

    // If endptr is not pointing to the end, the string is not fully numeric
    if (*endptr != '\0') return 0;

    *out_num = (int)val;
    return 1;
}
