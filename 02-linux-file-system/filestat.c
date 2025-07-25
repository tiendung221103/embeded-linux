#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>

/**
 * @brief Prints the type of a file based on the mode value.
 * 
 * @param [in] mode File mode obtained from stat/lstat, used to determine file type.
 */
void print_file_type(mode_t mode) {
    if (S_ISREG(mode)) {
        printf("File Type     : Regular File\n");
    } else if (S_ISDIR(mode)) {
        printf("File Type     : Directory\n");
    } else if (S_ISLNK(mode)) {
        printf("File Type     : Symbolic Link\n");
    } else {
        printf("File Type     : Other\n");
    }
}

/**
 * @brief Main function that retrieves and prints information about a specified file.
 *        Information includes file type, size, and last modified time.
 * 
 * @param [in] argc Argument count (must be 2).
 * @param [in] argv Argument vector. Expects a single file path as the second argument.
 * 
 * @return 0 on success, 1 on failure (e.g., invalid arguments or lstat error).
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        return 1;
    }

    const char *file_path = argv[1];
    struct stat fileStat;

    if (lstat(file_path, &fileStat) == -1) {
        perror("lstat");  
        return 1;
    }

    printf("File Path     : %s\n", file_path);
    print_file_type(fileStat.st_mode);
    printf("Size          : %lld bytes\n", (long long) fileStat.st_size);

    // Print last modification time
    char timebuf[100];
    struct tm *tm_info = localtime(&fileStat.st_mtime);
    strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("Last Modified : %s\n", timebuf);

    return 0;
}
