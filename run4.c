#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>

void performRead(const char *filename, int block_size, int block_count, const char *read_type) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("Error opening file");
        return;
    }

    char *buffer = malloc(block_size);
    if (buffer == NULL) {
        perror("Memory allocation failed");
        close(fd);
        return;
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);

    for (int i = 0; i < block_count; i++) {
        if (read(fd, buffer, block_size) < 0) {
            perror("Error reading file");
            break;
        }
    }

    gettimeofday(&end, NULL);
    close(fd);

    double time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;

    if (time_taken <= 0) {
        printf("Time taken for the operation is too small to measure.\n");
    } else {
        double file_size_mib = (double)block_size * block_count / (1024 * 1024);
        double performance = file_size_mib / time_taken;
        printf("%s Read - Performance: %.2f MiB/s\n", read_type, performance);
    }

    free(buffer);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <filename> <block_size> <block_count>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    int block_size = atoi(argv[2]);
    int block_count = atoi(argv[3]);

    // Perform the uncached read
    performRead(filename, block_size, block_count, "Uncached");

    // Manually clear caches here before running the program again

    // Perform the cached read (you need to run the program again after clearing caches)
    // performRead(filename, block_size, block_count, "Cached");

    return 0;
}
