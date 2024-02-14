#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <filename> <block_size> <block_count>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    int block_size = atoi(argv[2]);
    int block_count = atoi(argv[3]);
    char *buffer = malloc(block_size);
    if (buffer == NULL) {
        perror("Memory allocation failed");
        return 1;
    }

    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("Error opening file");
        free(buffer);
        return 1;
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);

    for (int i = 0; i < block_count; i++) {
        if (read(fd, buffer, block_size) < 0) {
            perror("Error reading file");
            close(fd);
            free(buffer);
            return 1;
        }
    }

    gettimeofday(&end, NULL);
    close(fd);

    double time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;

    if (time_taken <= 0) {
    printf("Time taken for the operation is too small to measure.\n");
    free(buffer);
    return 1;
    }

    double file_size_mib = (double)block_size * block_count / (1024 * 1024);
    double performance = file_size_mib / time_taken;

    printf("Performance: %.2f MiB/s\n", performance);

    free(buffer);
    return 0;
}
