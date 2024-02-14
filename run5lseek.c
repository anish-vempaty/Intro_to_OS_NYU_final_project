#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    // Open the file
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    off_t fileSize = lseek(fd, 0, SEEK_END);
    if (fileSize == -1) {
        perror("Error seeking file");
        close(fd);
        return 1;
    }

    struct timespec start, end;
    long long operations = 0;

    // Start the timer
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Use lseek to jump through the file
    for (off_t i = 0; i < fileSize; ++i) {
        if (lseek(fd, i, SEEK_SET) == -1) {
            perror("Error seeking file");
            close(fd);
            return 1;
        }
        operations++;
    }

    // End the timer
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Calculate the elapsed time
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    // Calculate performance
    double performanceOps = operations / elapsed;

    printf("Performance (Operations/s): %f\n", performanceOps);

    // Close the file
    close(fd);

    return 0;
}
