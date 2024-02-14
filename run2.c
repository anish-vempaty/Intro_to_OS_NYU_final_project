#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <filename> <block_size>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    int block_size = atoi(argv[2]);

    struct stat file_stat;
    if (stat(filename, &file_stat) < 0) {
        perror("Error getting file stats");
        return 1;
    }

    off_t file_size = file_stat.st_size;
    int block_count = file_size / block_size;

    printf("Block count for file size: %d\n", block_count);

    return 0;
}
