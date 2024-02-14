#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

double getTime() {
    struct timeval time;
    gettimeofday(&time, NULL);
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

void readFromFile(const char* filename, int block_size) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file for reading");
        exit(EXIT_FAILURE);
    }

    char* buffer = malloc(block_size);
    double start = getTime();
    while (read(fd, buffer, block_size) > 0) {
        // Reading data...
    }
    double end = getTime();
    free(buffer);
    close(fd);

    printf("Time taken to read the file: %.6f seconds\n", end - start);
}

void clearCache() {
    // Clear the cache - requires root access
    system("sudo sh -c 'echo 3 > /proc/sys/vm/drop_caches'");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <block_size>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *filename = argv[1];
    int block_size = atoi(argv[2]);

    // First read (potentially with warm cache)
    printf("Reading with warm cache...\n");
    readFromFile(filename, block_size);

    // Clear cache and read again
    printf("Clearing cache and reading again...\n");
    clearCache();
    readFromFile(filename, block_size);

    return 0;
}
