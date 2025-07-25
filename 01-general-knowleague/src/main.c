#include <stdio.h>
#include "strutils.h"

/**
 * @brief Demonstrates the usage of strutils functions:
 *        str_trim, str_reverse, and str_to_int.
 * 
 * @return Exit status code (0 = success).
 */
int main() {
    char str1[] = "  Hello World!  ";
    char str2[] = "12345";
    char str3[] = "Test";

    // Demonstrate trimming whitespace
    printf("Original str1: '%s'\n", str1);
    str_trim(str1);
    printf("Trimmed str1: '%s'\n", str1);

    // Demonstrate string reversal
    printf("Original str3: '%s'\n", str3);
    str_reverse(str3);
    printf("Reversed str3: '%s'\n", str3);

    // Demonstrate string to integer conversion
    int num;
    if (str_to_int(str2, &num)) {
        printf("Converted '%s' to number: %d\n", str2, num);
    } else {
        printf("Failed to convert '%s' to number.\n", str2);
    }

    return 0;
}
