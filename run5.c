#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define ONE_BYTE 1

// Function to calculate the elapsed time in seconds
double time_elapsed(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

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

    char buffer[ONE_BYTE];
    int bytesRead;
    long long totalBytesRead = 0;
    struct timespec start, end;

    // Start the timer
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Read the file 1 byte at a time
    while ((bytesRead = read(fd, buffer, ONE_BYTE)) > 0) {
        totalBytesRead += bytesRead;
    }

    // End the timer
    clock_gettime(CLOCK_MONOTONIC, &end);

    if (bytesRead == -1) {
        perror("Error reading file");
        close(fd);
        return 1;
    }

    // Calculate the elapsed time
    double elapsed = time_elapsed(start, end);

    // Calculate performance
    double performanceMiBs = totalBytesRead / (elapsed * 1024 * 1024);
    double performanceBs = totalBytesRead / elapsed;

    printf("Performance (MiB/s): %f\n", performanceMiBs);
    printf("Performance (B/s): %f\n", performanceBs);

    // Close the file
    close(fd);

    return 0;
}
